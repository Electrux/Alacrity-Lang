/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef PARSER_FN_CALL_STMT_HPP
#define PARSER_FN_CALL_STMT_HPP

#include <vector>
#include <string>
#include <optional>

#include "Stmt.hpp"
#include "BlockStmt.hpp"
#include "../Lexer/Lexer.hpp"

namespace Parser
{
class FnCallStmt : public Stmt
{
	// vector to accomodate blocks, for example:
	// a function in project function's block will
	// have "project", "<fname>" in this vector
	std::vector< std::string > m_name;
	std::string m_full_name;
	std::string m_lib_name;
	std::vector< std::string > m_args;
	const BlockStmt * m_block; // may or may not exist for any given function call
	// for example: for the project() function call, it exists (optionally),
	// for use_lib, it does not
	explicit FnCallStmt( const std::vector< std::string > & name, const std::string & full_name, const std::string & lib_name,
		const std::vector< std::string > & args, const BlockStmt * block );
public:
	~FnCallStmt();
	static FnCallStmt * Parse( const LexSymVec & tokens, int & loc, std::vector< std::string > parent_funcs );

	void Disp( const bool has_next = false ) const;

	const std::string & GetName() const;
	const std::string & GetLibName() const;
	const std::string & GetFullName() const;
	const std::vector< std::string > & GetNameVec() const;
	const std::vector< std::string > & GetArgs() const;
	const BlockStmt * GetBlock() const;
};
}

#endif // PARSER_FN_CALL_STMT_HPP