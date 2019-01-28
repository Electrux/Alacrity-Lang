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
#include <variant>

#include "../../include/Errors.hpp"
#include "../../include/IO.hpp"
#include "../../include/Lexer/Lexer.hpp"
#include "../../include/Parser/Stmt.hpp"
#include "../../include/Parser/FnCallStmt.hpp"
#include "../../include/Parser/CondStmt.hpp"
#include "../../include/Parser/LoopStmt.hpp"
#include "../../include/Parser/BlockStmt.hpp"

Parser::BlockStmt::BlockStmt( const std::vector< Stmt * > & stmts )
	: Stmt( BLOCK ), m_stmts( stmts ) {}

Parser::BlockStmt::~BlockStmt()
{
	for( auto & s : m_stmts ) {
		delete s;
	}
}

std::variant< int, std::vector< Parser::Stmt * > > Parser::BlockStmt::Parse( const LexSymVec & tokens, int & loc, std::vector< std::string > parent_funcs )
{
	std::string err;
	int err_line, err_col;
	int err_val;

	std::vector< Stmt * > stmts;

	if( tokens[ loc ]->GetType() == Lex::SEPAR && tokens[ loc ]->GetDetailType() == Lex::BRACEOP ) {
		++loc;
	}

	while( loc < tokens.size() && ( tokens[ loc ]->GetType() != Lex::SEPAR || tokens[ loc ]->GetDetailType() != Lex::BRACECL ) ) {
		err_line = tokens[ loc ]->GetLine();
		err_col = tokens[ loc ]->GetCol();
		if( tokens[ loc ]->GetType() == Lex::STR ) {
			if( loc + 1 >= tokens.size() ) {
				err = "Expected parenthesis open after " + tokens[ loc ]->GetData() + " , but found <EOF>";
				err_val = EARLY_EOF;
				goto error;
			}
			++loc;
			err_line = tokens[ loc ]->GetLine();
			err_col = tokens[ loc ]->GetCol();
			// Well, it's a function call!
			if( tokens[ loc ]->GetType() == Lex::SEPAR && tokens[ loc ]->GetDetailType() == Lex::PARENOP ) {
				// Go to function name
				--loc;
				int tmp_loc = loc;
				FnCallStmt * fncall = FnCallStmt::Parse( tokens, loc, parent_funcs );
				if( fncall == nullptr ) {
					err = "Error encountered while parsing function: " + tokens[ tmp_loc ]->GetData();
					err_val = FN_PARSE_FAIL;
					goto error;
				}
				stmts.push_back( fncall );
			} else {
				err = "Expected parenthesis open after " + tokens[ loc - 1 ]->GetData()
					+ " , but found " + tokens[ loc ]->GetData();
				err_val = UNEXPECTED_TOKEN;
				goto error;
			}
		} else if( tokens[ loc ]->GetType() == Lex::SEPAR && tokens[ loc ]->GetDetailType() == Lex::BRACEOP ) {
			auto block_var = Parse( tokens, loc, parent_funcs );
			if( std::holds_alternative< int >( block_var ) ) {
				err_val = std::get< int >( block_var );
				err = "Error encountered while parsing block";
				goto error;
			} else {
				auto block_stmts = std::get< std::vector< Stmt * > >( block_var );
				stmts.push_back( new BlockStmt( block_stmts ) );
			}
		} else if( tokens[ loc ]->GetType() == Lex::KEYW && tokens[ loc ]->GetDetailType() == Lex::IF ) {
			auto cond_var = CondStmt::Parse( tokens, loc, parent_funcs );
			if( std::holds_alternative< int >( cond_var ) ) {
				err_val = std::get< int >( cond_var );
				err = "Error encountered while parsing conditional";
				goto error;
			} else {
				auto cond = std::get< CondStmt * >( cond_var );
				stmts.push_back( cond );
			}
		} else if( tokens[ loc ]->GetType() == Lex::KEYW && ( 
				tokens[ loc ]->GetDetailType() == Lex::FOR ||
				tokens[ loc ]->GetDetailType() == Lex::FOREACH ||
				tokens[ loc ]->GetDetailType() == Lex::FOREACHVAR ) ) {
			int tmp_loc = loc;
			Parser::LoopType type;
			if( tokens[ loc ]->GetDetailType() == Lex::FOR ) 	type = FOR;
			if( tokens[ loc ]->GetDetailType() == Lex::FOREACH ) 	type = FOREACH;
			if( tokens[ loc ]->GetDetailType() == Lex::FOREACHVAR ) type = FOREACHVAR;
			LoopStmt * loop_var = LoopStmt::Parse( tokens, loc, parent_funcs, type );
			if( loop_var == nullptr ) {
				err = "Error encountered while parsing: " + tokens[ tmp_loc ]->GetData();
				err_val = FN_PARSE_FAIL;
				goto error;
			}
			stmts.push_back( loop_var );
		} else {
			err = "Unexpected symbol: " + tokens[ loc ]->GetData() + " occured";
			goto error;
		}
		++loc;
	}
	return stmts;
error:
	IO::out.IncTab( false );
	IO::out() << "Error: " << err << "; on line: " << err_line << "[" << err_col << "]\n";
	IO::out.DecTab();
	return err_val;
}

Parser::BlockStmt * Parser::BlockStmt::GenBlock( const std::vector< Stmt * > stmts )
{
	return new BlockStmt( stmts );
}

void Parser::BlockStmt::Disp( const bool has_next ) const
{
	IO::out.IncTab( has_next );
	IO::out( has_next ) << " Block at <" << this << ">:\n";

	for( int i = 0; i < m_stmts.size(); ++i ) {
		m_stmts[ i ]->Disp( i != m_stmts.size() - 1 );
	}

	IO::out.DecTab();
}

const std::vector< Parser::Stmt * > & Parser::BlockStmt::GetStmts() const { return m_stmts; }

size_t Parser::BlockStmt::size() const { return m_stmts.size(); }