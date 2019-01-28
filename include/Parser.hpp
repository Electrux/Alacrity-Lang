/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef PARSER_HPP
#define PARSER_HPP

#include <variant>

#include "Lexer/Lexer.hpp"
#include "Parser/Stmt.hpp"

namespace Parser
{
class ParseTree
{
	std::vector< Stmt * > m_stmts;
public:
	ParseTree();
	ParseTree( std::vector< Stmt * > stmts );
	ParseTree( ParseTree && original );
	ParseTree( ParseTree & original );
	ParseTree( ParseTree const & original ) = delete;
	ParseTree & operator =( ParseTree original );

	friend void swap( ParseTree & stmt1, ParseTree & stmt2 );

	Stmt * operator []( const size_t loc );

	void push_back( Stmt * const stmt );
	void pop_back();
	size_t size() const;

	// iteration
	const std::vector< Stmt * > & GetStmts() const;

	~ParseTree();
};
void swap( ParseTree & stmt1, ParseTree & stmt2 );

std::variant< int, ParseTree > ParseTokens( const LexSymVec & tokens );
}

#endif // PARSER_HPP