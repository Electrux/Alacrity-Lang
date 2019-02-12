/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef PARSER_BLOCK_STMT_HPP
#define PARSER_BLOCK_STMT_HPP

#include <string>
#include <optional>

#include "Stmt.hpp"
#include "../Lexer/Lexer.hpp"

namespace Parser
{
class BlockStmt : public Stmt
{
	std::vector< Stmt * > m_stmts;
	explicit BlockStmt( const std::vector< Stmt * > & stmts );
public:
	~BlockStmt();
	static std::variant< int, std::vector< Parser::Stmt * > > Parse( const LexSymVec & tokens, size_t & loc, std::vector< std::string > parent_funcs );
	static BlockStmt * GenBlock( const std::vector< Stmt * > & stmts );

	void Disp( const bool has_next = false ) const;

	const std::vector< Stmt * > & GetStmts() const;
	size_t size() const;
};
}

#endif // PARSER_BLOCK_STMT_HPP