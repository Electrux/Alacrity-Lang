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
#include "../../include/Parser/CondStmt.hpp"

void Parser::Operation::clear()
{
	type = SIMPLE;
	lhs.clear();
	rhs.clear();
	oper.clear();
}

void Parser::Operation::Disp( const bool has_next ) const
{
	IO::out.IncTab( has_next );
	IO::out( has_next ) << " Operation at <" << this << ">:\n";
	IO::out.IncTab( has_next );
	if( type == SIMPLE ) {
		IO::out( true ) << " Type: simple\n";
	} else if( type == AND ) {
		IO::out( true ) << " Type: logical and\n";
	} else if( type == OR ) {
		IO::out( true ) << " Type: logical or\n";
	}
	IO::out( true ) << " LHS: \"" << lhs << "\"\n";
	IO::out( true ) << " RHS: \"" << rhs << "\"\n";
	IO::out( false ) << " OPER: " << oper << "\n";
	IO::out.DecTab( 2 );
}

void Parser::Cond::clear()
{
	ops.clear();
	block = nullptr;
}

Parser::CondStmt::CondStmt( const std::vector< Cond > & conds ) : Stmt( COND ), m_conds( conds ) {}

Parser::CondStmt::~CondStmt()
{
	for( auto & c : m_conds ) {
		delete c.block;
	}
}

void Parser::CondStmt::AddBlock( const Cond & cond ) { m_conds.push_back( cond ); }
const Parser::Cond & Parser::CondStmt::operator []( const size_t i ) const { return m_conds[ i ]; }

std::variant< int, Parser::CondStmt * > Parser::CondStmt::Parse( const LexSymVec & tokens, int & loc, std::vector< std::string > parent_funcs )
{
	std::string err;
	int err_line, err_col;
	int err_val;
	CondType ctype;

	std::vector< Cond > conds;
	Cond cond_tmp;
begin:
	if( tokens[ loc ]->GetType() == Lex::KEYW && ( tokens[ loc ]->GetDetailType() == Lex::IF ||
							tokens[ loc ]->GetDetailType() == Lex::ELIF ||
							tokens[ loc ]->GetDetailType() == Lex::ELSE ) ) {
		if( tokens[ loc ]->GetDetailType() == Lex::IF ) ctype = IF;
		else if( tokens[ loc ]->GetDetailType() == Lex::ELIF ) ctype = ELIF;
		else ctype = ELSE;
		++loc;
	}
	if( loc >= tokens.size() ) {
		err = "Expected '(' or <block>, but found <EOF>";
		goto error_size;
	}
	if( ctype != ELSE && tokens[ loc ]->GetType() == Lex::SEPAR && tokens[ loc ]->GetDetailType() == Lex::PARENOP ) {
		++loc;
	}
	if( ctype != ELSE ) {
		Operation op;
		op.type = SIMPLE;
op_begin:
		if( loc >= tokens.size() ) {
			err = "Expected <LHS>, but found <EOF>";
			goto error_size;
		}
		op.lhs = tokens[ loc ]->GetData();
		++loc;
		if( loc >= tokens.size() ) {
			err = "Expected <OPER>, but found <EOF>";
			goto error_size;
		}
		op.oper = tokens[ loc ]->GetData();
		++loc;
		if( loc >= tokens.size() ) {
			err = "Expected <RHS>, but found <EOF>";
			goto error_size;
		}
		op.rhs = tokens[ loc ]->GetData();
		++loc;
		if( loc >= tokens.size() ||
			( ( tokens[ loc ]->GetType() != Lex::SEPAR || tokens[ loc ]->GetDetailType() != Lex::PARENCL ) &&
				( tokens[ loc ]->GetType() != Lex::LOGIC || (
				tokens[ loc ]->GetDetailType() != Lex::AND && tokens[ loc ]->GetDetailType() != Lex::OR ) ) ) ) {
			err = "Expected ')', '||' or '&&', but found " + ( ( loc >= tokens.size() ) ? " <EOF>" : tokens[ loc ]->GetData() );
			goto error_size;
		}
		cond_tmp.ops.push_back( op );
		op.clear();
		if( tokens[ loc ]->GetType() == Lex::LOGIC &&
				( tokens[ loc ]->GetDetailType() == Lex::AND ||
				tokens[ loc ]->GetDetailType() == Lex::OR ) ) {
			int d_type = tokens[ loc ]->GetDetailType();
			if( d_type == Lex::AND ) {
				op.type = AND;
			} else if( d_type == Lex::OR ) {
				op.type = OR;
			}
			++loc;
			goto op_begin;
		}
		++loc;
	}
	if( loc >= tokens.size() || ( tokens[ loc ]->GetType() != Lex::SEPAR || tokens[ loc ]->GetDetailType() != Lex::BRACEOP ) ) {
		err = "Expected '{', but found <EOF>";
		goto error_size;
	} else {
		auto block_var = BlockStmt::Parse( tokens, loc, parent_funcs );
		if( std::holds_alternative< int >( block_var ) ) {
			err = "Error encountered while parsing block from conditional\n";
			err_val = std::get< int >( block_var );
			err_line = tokens[ loc ]->GetLine();
			err_col = tokens[ loc ]->GetCol();
			goto error;
		} else {
			auto block_stmts = std::get< std::vector< Stmt * > >( block_var );
			cond_tmp.block = BlockStmt::GenBlock( block_stmts );
			conds.push_back( cond_tmp );
			cond_tmp.clear();
		}
	}
	if( loc + 1 < tokens.size() && tokens[ loc + 1 ]->GetType() == Lex::KEYW &&
			( tokens[ loc + 1 ]->GetDetailType() == Lex::ELIF ||
			tokens[ loc + 1 ]->GetDetailType() == Lex::ELSE ) ) {
		++loc;
		goto begin;
	}
end:
	return new CondStmt( conds );
error_size:
	err_line = tokens[ loc - 1 ]->GetLine();
	err_col = tokens[ loc - 1 ]->GetCol();
	err_val = EARLY_EOF;
error:
	IO::out.IncTab( false );
	IO::out() << "Error: " << err << "; on line: " << err_line << "[" << err_col << "]\n";
	IO::out.DecTab();
	return err_val;
}

void Parser::CondStmt::Disp( const bool has_next ) const
{
	IO::out.IncTab( has_next );
	IO::out( has_next ) << " Conditional at <" << this << ">:\n";

	for( int i = 0; i < m_conds.size(); ++i ) {
		bool next = i != m_conds.size() - 1;
		IO::out.IncTab( next );
		if( i == 0 ) {
			IO::out( next ) << " If:\n";
		} else if( !m_conds[ i ].ops.empty() ) {
			IO::out( next ) << " Elif:\n";
		} else {
			IO::out( next ) << " Else:\n";
		}
		for( int j = 0; j < m_conds[ i ].ops.size(); ++j ) {
			m_conds[ i ].ops[ j ].Disp( true );
		}
		m_conds[ i ].block->Disp( false );
		IO::out.DecTab();
	}

	IO::out.DecTab();
}

const std::vector< Parser::Cond > & Parser::CondStmt::GetConds() const { return m_conds; }

size_t Parser::CondStmt::size() const { return m_conds.size(); }