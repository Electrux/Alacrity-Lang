/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef INTERPRETER_BLOCK_HPP
#define INTERPRETER_BLOCK_HPP

#include "../Parser.hpp"
#include "../Parser/BlockStmt.hpp"

namespace Interpreter
{
int Block( const Parser::BlockStmt * blk, const size_t depth, const bool internal_display_enabled );
}

#endif // INTERPRETER_BLOCK_HPP