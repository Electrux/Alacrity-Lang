/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef INTERPRETER_ASSIGNMENT_HPP
#define INTERPRETER_ASSIGNMENT_HPP

#include "../Parser.hpp"
#include "../Parser/AssignStmt.hpp"

namespace Interpreter
{
int Assignment( const Parser::AssignStmt * assn_var, const size_t depth );
}

#endif // INTERPRETER_ASSIGNMENT_HPP