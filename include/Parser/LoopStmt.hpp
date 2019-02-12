/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef PARSER_LOOP_STMT_HPP
#define PARSER_LOOP_STMT_HPP

#include <vector>
#include <string>
#include <optional>

#include "Stmt.hpp"
#include "BlockStmt.hpp"
#include "../Lexer/Lexer.hpp"

namespace Parser
{
enum LoopType
{
	FOR,
	FOREACH,
	FOREACHVAR,
};

// Loop

class LoopStmt : public Stmt
{
	std::vector< std::string > m_args;
	const BlockStmt * m_block;
	const LoopType m_loop_type;
	explicit LoopStmt( const std::vector< std::string > & args, const BlockStmt * block, const LoopType loop_type );
public:
	~LoopStmt();
	static LoopStmt * Parse( const LexSymVec & tokens, size_t & loc, std::vector< std::string > parent_funcs, const LoopType loop_type );

	void Disp( const bool has_next = false ) const;

	const std::vector< std::string > & GetArgs() const;
	const BlockStmt * GetBlock() const;
	LoopType GetType() const;
};
}

#endif // PARSER_LOOP_STMT_HPP