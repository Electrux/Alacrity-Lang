/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef PARSER_ASSIGN_STMT_HPP
#define PARSER_ASSIGN_STMT_HPP

#include <vector>
#include <string>
#include <optional>

#include "Stmt.hpp"
#include "BlockStmt.hpp"
#include "../Lexer/Lexer.hpp"

namespace Parser
{
class AssignStmt : public Stmt
{
	std::string m_var;
	std::string m_val;
	AssignStmt( const std::string & var, const std::string & val );
public:
	~AssignStmt();
	static AssignStmt * Parse( const LexSymVec & tokens, int & loc );

	void Disp( const bool has_next = false ) const;

	const std::string & GetVar() const;
	const std::string & GetVal() const;
};
}

#endif // PARSER_ASSIGN_STMT_HPP