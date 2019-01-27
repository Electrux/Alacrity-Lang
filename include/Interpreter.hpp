/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "Parser.hpp"

namespace Interpreter
{
int Interpret( const Parser::ParseTree & ps, const std::string & file_path,
		const int depth = 0, const bool enable_internal_display = true );
}

#endif // INTERPRETER_HPP