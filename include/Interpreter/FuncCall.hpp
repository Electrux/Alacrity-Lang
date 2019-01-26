/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef INTERPRETER_FUNC_CALL_HPP
#define INTERPRETER_FUNC_CALL_HPP

#include "../Parser.hpp"
#include "../Parser/FnCallStmt.hpp"

namespace Interpreter
{
int FuncCall( const Parser::FnCallStmt * fncall, const size_t depth );
}

#endif // INTERPRETER_FUNC_CALL_HPP