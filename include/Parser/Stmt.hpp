/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef PARSER_STMT_HPP
#define PARSER_STMT_HPP

#include <vector>
#include <string>

namespace Parser
{

enum StmtType
{
	BLOCK,
	FNCALL,
	COND,
	LOOP,
	LOOP_IN_VAR,
};

// Base class for all statement types

class Stmt
{
	int m_type;
public:
	Stmt( int type );
	virtual ~Stmt() {}
	virtual void Disp( const bool has_next = false ) const = 0;

	int GetType() const;
};
}

#endif // PARSER_STMT_HPP