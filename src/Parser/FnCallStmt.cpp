/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <vector>
#include <string>
#include <optional>

#include "../../include/Errors.hpp"
#include "../../include/Core.hpp"
#include "../../include/IO.hpp"
#include "../../include/Lexer/Lexer.hpp"
#include "../../include/Parser/Stmt.hpp"
#include "../../include/Parser/BlockStmt.hpp"
#include "../../include/Parser/FnCallStmt.hpp"

static void GetActualFunctionName( std::vector< std::string > & parents, std::string name );

Parser::FnCallStmt::FnCallStmt( const std::vector< std::string > & name, const std::string & full_name, const std::string & lib_name,
				const std::vector< std::string > & args, const BlockStmt * block )
	: Stmt( FNCALL ), m_name( name ), m_full_name( full_name ), m_lib_name( lib_name ), m_args( args ), m_block( block ) {}

Parser::FnCallStmt::~FnCallStmt()
{
	if( m_block != nullptr ) {
		delete m_block;
	}
}

Parser::FnCallStmt * Parser::FnCallStmt::Parse( const LexSymVec & tokens, size_t & loc, std::vector< std::string > parent_funcs )
{
	GetActualFunctionName( parent_funcs, tokens[ loc ]->GetData() );
	std::string full_name, lib_name = Core::FuncLibFile( parent_funcs[ 0 ] );
	for( auto it = parent_funcs.begin(); it != parent_funcs.end(); ++it ) {
		full_name += ( it == parent_funcs.end() - 1 ) ? * it : * it + "/";
	}
	std::vector< std::string > fn_args;
	BlockStmt * fn_block = nullptr;
	std::string err;
	int err_line, err_col;
	bool done = true;
	std::string tmpstr;
	++loc;
	if( loc >= tokens.size() || ( tokens[ loc ]->GetType() != Lex::SEPAR || tokens[ loc ]->GetDetailType() != Lex::PARENOP ) ) {
		if( loc >= tokens.size() ) {
			err = "Expected <Parenthesis open>, but found <EOF>";
		} else {
			err = "Expected <Parenthesis open>, but found " + tokens[ loc ]->GetData();
		}
		int tmp_loc = loc >= tokens.size() ? loc - 1 : loc;
		err_line = tokens[ tmp_loc ]->GetLine();
		err_col = tokens[ tmp_loc ]->GetCol();
		goto error;
	}
	// one more ++loc for moving ahead of parenthesis
	++loc;
	while( loc < tokens.size() && ( tokens[ loc ]->GetType() != Lex::SEPAR || tokens[ loc ]->GetDetailType() != Lex::PARENCL ) ) {
		done = false;
		err_line = tokens[ loc ]->GetLine();
		err_col = tokens[ loc ]->GetCol();
		if( tokens[ loc ]->GetType() == Lex::STR || tokens[ loc ]->GetType() == Lex::NUM ) {
			tmpstr = tokens[ loc ]->GetData();
			done = true;
			fn_args.push_back( tmpstr );
			tmpstr.clear();
		} else if( tokens[ loc ]->GetType() == Lex::SEPAR && tokens[ loc ]->GetDetailType() == Lex::COMMA ) {
			++loc;
			continue;
		} else {
			err = "Expected <identifier> or ',', but found: " + tokens[ loc ]->GetData();
			err_line = tokens[ loc ]->GetLine();
			err_col = tokens[ loc ]->GetCol();
			goto error;
		}
		++loc;
	}
	if( !done ) {
		err = "Unfinished function call parse! Unknown error (probably parameter related)!";
		goto error;
	}
	if( loc + 1 >= tokens.size() ) {
		return new FnCallStmt( parent_funcs, full_name, lib_name, fn_args, nullptr );
	}
	if( tokens[ loc + 1 ]->GetType() == Lex::SEPAR && tokens[ loc + 1 ]->GetDetailType() == Lex::BRACEOP ) {
		++loc;
		err_line = tokens[ loc ]->GetLine();
		err_col = tokens[ loc ]->GetCol();
		auto block_var = BlockStmt::Parse( tokens, loc, parent_funcs );
		if( std::holds_alternative< int >( block_var ) ) {
			err = "Error encountered while parsing block from function call of: " + full_name;
			goto error;
		} else {
			fn_block = BlockStmt::GenBlock( std::get< std::vector< Stmt * > >( block_var ) );
		}
	}
	return new FnCallStmt( parent_funcs, full_name, lib_name, fn_args, fn_block );
error:
	IO::out.IncTab( false );
	IO::out() << "Error: " << err << "; on line: " << err_line << "[" << err_col << "]\n";
	IO::out.DecTab();
	return nullptr;
}

void Parser::FnCallStmt::Disp( const bool has_next ) const
{
	IO::out.IncTab( has_next );
	IO::out( has_next ) << " Function Call at <" << this << ">:\n";
	IO::out.IncTab( true );

	IO::out( true ) << " name: \"" << m_full_name << "\"\n";
	IO::out( true ) << " lib name: \"" << m_lib_name << "\"\n";
	IO::out( true ) << " args:\n";

	IO::out.IncTab( true );
	for( size_t i = 0; i < m_args.size(); ++i ) {
		IO::out( i != m_args.size() - 1 ) << " \"" << m_args[ i ] << "\"\n";
	}
	IO::out.DecTab();

	IO::out.DecTab();
	IO::out.IncTab( false );
	if( m_block == nullptr ) {
		IO::out() << " block: NULL\n";
	} else {
		IO::out() << " block:\n";
		m_block->Disp( false );
	}

	IO::out.DecTab( 2 );
}

const std::string & Parser::FnCallStmt::GetName() const { return * ( m_name.end() - 1 ); }
const std::string & Parser::FnCallStmt::GetLibName() const { return m_lib_name; }
const std::string & Parser::FnCallStmt::GetFullName() const { return m_full_name; }
const std::vector< std::string > & Parser::FnCallStmt::GetNameVec() const { return m_name; }
const std::vector< std::string > & Parser::FnCallStmt::GetArgs() const { return m_args; }
const Parser::BlockStmt * Parser::FnCallStmt::GetBlock() const { return m_block; }

static void GetActualFunctionName( std::vector< std::string > & parents, std::string name )
{
	size_t loc = name.find( '.' );
	if( loc != std::string::npos ) {
		parents.clear();
		while( loc != std::string::npos ) {
			if( loc > 0 ) parents.push_back( name.substr( 0, loc ) );
			name = name.substr( loc + 1 );
			loc = name.find( '.' );
		}
		if( !name.empty() ) parents.push_back( name );
	} else {
		parents.push_back( name );
	}
}