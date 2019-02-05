/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef INTERPRETER_LOOPS_HPP
#define INTERPRETER_LOOPS_HPP

#include "../Parser.hpp"
#include "../Parser/LoopStmt.hpp"

namespace Interpreter
{
int LoopCall( const Parser::LoopStmt * loop_var, const size_t depth, const bool internal_display_enabled );
}

#endif // INTERPRETER_LOOPS_HPP