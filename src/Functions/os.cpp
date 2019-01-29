/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <iostream>
#include <chrono>
#include <thread>

#include "../../include/Errors.hpp"
#include "../../include/Core.hpp"
#include "../../include/String.hpp"
#include "../../include/Env.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

AL_FUNC_FIX_ARG( sleep, 1, false, false )
{
	std::string dur_str;
	EVAL_AND_CHECK( "sleep", args[ 0 ], dur_str );
	if( !Str::IsNum( dur_str ) ) {
		std::cerr << "Expected a number for sleep duration\n";
		return FAIL;
	}
	std::this_thread::sleep_for( std::chrono::milliseconds( std::stoul( dur_str ) ) );
	return OK;
}