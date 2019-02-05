/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef INTERPRETER_CONDITIONAL_HPP
#define INTERPRETER_CONDITIONAL_HPP

#include "../Parser.hpp"
#include "../Parser/CondStmt.hpp"

namespace Interpreter
{
int Conditional( const Parser::CondStmt * conds, const size_t depth, const bool internal_display_enabled );
}

#endif // INTERPRETER_CONDITIONAL_HPP