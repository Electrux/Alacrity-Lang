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
#include "../../include/Parser/AssignStmt.hpp"

Parser::AssignStmt::AssignStmt( const std::string & var, const std::string & val )
	: Stmt( ASSIGN ), m_var( var ), m_val( val ) {}

Parser::AssignStmt::~AssignStmt() {}

Parser::AssignStmt * Parser::AssignStmt::Parse( const LexSymVec & tokens, int & loc )
{
	// var
	std::string var = tokens[ loc ]->GetData(), val;
	std::string err;
	int err_line = tokens[ loc ]->GetLine(), err_col = tokens[ loc ]->GetCol();
	++loc;
	// =
	if( tokens[ loc ]->GetType() != Lex::KEYW || tokens[ loc ]->GetDetailType() != Lex::ASSIGN ) {
		err = "Expected assignment (=) operator but found " + tokens[ loc ]->GetData();
		goto error;
	}
	++loc;
	// val
	if( loc >= tokens.size() ) {
		err = "Expected value for variable " + var + " but found <EOF>";
		goto error;
	}
	val = tokens[ loc ]->GetData();
	return new AssignStmt( var, val );
error:
	IO::out.IncTab( false );
	IO::out() << "Error: " << err << "; on line: " << err_line << "[" << err_col << "]\n";
	IO::out.DecTab();
	return nullptr;
}

void Parser::AssignStmt::Disp( const bool has_next ) const
{
	IO::out.IncTab( has_next );
	IO::out( has_next ) << " Assignment at <" << this << ">:\n";
	IO::out.IncTab( true );

	IO::out( true ) << " variable: " << m_var << "\n";
	IO::out( false ) << " value: " << m_val << "\n";

	IO::out.DecTab( 2 );
}

const std::string & Parser::AssignStmt::GetVar() const { return m_var; }
const std::string & Parser::AssignStmt::GetVal() const { return m_val; }