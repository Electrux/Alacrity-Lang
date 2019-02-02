/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef PARSER_COND_STMT_HPP
#define PARSER_COND_STMT_HPP

#include <string>
#include <optional>

#include "Stmt.hpp"
#include "BlockStmt.hpp"
#include "../Lexer/Lexer.hpp"

namespace Parser
{
enum CondType
{
	IF,
	ELIF,
	ELSE,
};

enum OperationType
{
	SIMPLE,
	AND,
	OR,
};
struct Operation
{
	int type;
	std::string lhs;
	std::string rhs;
	std::string oper;
	void clear();
	void Disp( const bool has_next = false ) const;
};

struct Cond
{
	std::vector< Operation > ops;
	BlockStmt * block;
	void clear();
};

class CondStmt : public Stmt
{
	std::vector< Cond > m_conds;
	explicit CondStmt( const std::vector< Cond > & conds );
public:
	~CondStmt();
	void AddBlock( const Cond & cond );
	const Cond & operator []( const size_t i ) const;

	static std::variant< int, CondStmt * > Parse( const LexSymVec & tokens, int & loc, std::vector< std::string > parent_funcs );

	void Disp( const bool has_next = false ) const;

	const std::vector< Cond > & GetConds() const;
	size_t size() const;
};
}

#endif // PARSER_COND_STMT_HPP