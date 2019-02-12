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

#include "../include/IO.hpp"
#include "../include/Errors.hpp"
#include "../include/Parser/Stmt.hpp"
#include "../include/Parser/BlockStmt.hpp"
#include "../include/Parser/FnCallStmt.hpp"
#include "../include/Parser.hpp"

// ParseTree class

Parser::ParseTree::ParseTree() {}

Parser::ParseTree::ParseTree( const std::vector< Stmt * > & stmts ) : m_stmts( stmts ) {}

Parser::ParseTree::ParseTree( ParseTree && original ) : ParseTree()
{
	swap( * this, original );
}

Parser::ParseTree::ParseTree( ParseTree & original ) : m_stmts( original.m_stmts )
{
	original.m_stmts.clear();
}

Parser::ParseTree & Parser::ParseTree::operator =( ParseTree original )
{
	swap( * this, original );
	return * this;
}

// friend function
void Parser::swap( ParseTree & stmt1, ParseTree & stmt2 )
{
	using std::swap;
	swap( stmt1.m_stmts, stmt2.m_stmts );
}

Parser::Stmt * Parser::ParseTree::operator []( const size_t loc )
{
	if( loc >= m_stmts.size() ) return nullptr;
	return m_stmts[ loc ];
}

void Parser::ParseTree::push_back( Stmt * const stmt )
{
	m_stmts.push_back( stmt );
}

void Parser::ParseTree::pop_back()
{
	if( m_stmts.empty() ) return;
	if( * ( m_stmts.end() - 1 ) != nullptr ) delete * ( m_stmts.end() - 1 );
	m_stmts.pop_back();
}

size_t Parser::ParseTree::size() const
{
	return m_stmts.size();
}

// iteration
const std::vector< Parser::Stmt * > & Parser::ParseTree::GetStmts() const
{
	return m_stmts;
}

Parser::ParseTree::~ParseTree()
{
	for( auto & stmt : m_stmts ) {
		if( stmt != nullptr ) delete stmt;
	}
}

std::variant< int, Parser::ParseTree > Parser::ParseTokens( const LexSymVec & tokens )
{
	if( tokens.size() == 0 ) {
		IO::out.IncTab( false );
		IO::out() << "Error: Nothing to parse! (File empty)\n";
		IO::out.DecTab();
		return TOKENS_EMPTY;
	}
	int err_val = 0;
	std::string err;
	size_t loc = 0;
	auto block_var = BlockStmt::Parse( tokens, loc, {} );
	if( std::holds_alternative< int >( block_var ) ) {
		err_val = std::get< int >( block_var );
		err = "Error encountered while parsing from tokens";
		goto error;
	} else {
		ParseTree pt( std::get< std::vector< Stmt * > >( block_var ) );
		return pt;
	}
error:
	IO::out.IncTab( false );
	IO::out() << "Error: " << err << "\n";
	IO::out.DecTab();
	return err_val;
}