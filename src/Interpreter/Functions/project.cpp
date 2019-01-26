/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <iostream>
#include <string>

#include "../../../include/Errors.hpp"
#include "../../../include/String.hpp"
#include "../../../include/Env.hpp"
#include "../../../include/Interpreter/FnBase.hpp"

EBS_FUNC_FIX_ARG( project, 1, true, false )
{
	Env::SetVar( "PROJECT_NAME", args[ 0 ] );
	return OK;
}

EBS_FUNC_FIX_ARG( version, 1, true, false )
{
	Env::SetVar( "PROJECT_VERSION", args[ 0 ] );
	return OK;
}

EBS_FUNC_FIX_ARG( language, 2, true, false )
{
	Env::SetVar( "LANGUAGE", args[ 0 ] );
	Env::SetVar( "STD_VERSION", args[ 1 ] );
#if __linux__
	Env::SetVar( "CC", args[ 0 ] == "cpp" ? "g++" : "gcc" );
#elif __APPLE__
	Env::SetVar( "CC", args[ 0 ] == "cpp" ? "clang++" : "clang" );
#endif
	return OK;
}

EBS_FUNC_FIX_ARG( license, 1, true, false )
{
	Env::SetVar( "LICENSE", args[ 0 ] );
	return OK;
}

EBS_FUNC_FIX_ARG( author, 2, true, false )
{
	Env::SetVar( "AUTHOR_NAME", args[ 0 ] );
	Env::SetVar( "AUTHOR_EMAIL", args[ 1 ] );
	return OK;
}
