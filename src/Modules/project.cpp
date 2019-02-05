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

#include "../../include/Errors.hpp"
#include "../../include/Core.hpp"
#include "../../include/String.hpp"
#include "../../include/Env.hpp"
#include "../../include/FS.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

AL_FUNC_FIX_ARG( project, 1, true, false )
{
	std::string name;
	EVAL_AND_CHECK( "project", args[ 0 ], name );
	Env::SetVar( "PROJECT", name );
	return OK;
}

AL_FUNC_FIX_ARG( version, 1, false, false )
{
	std::string version;
	EVAL_AND_CHECK( "version", args[ 0 ], version );
	Env::SetVar( "VERSION", version );
	return OK;
}

AL_FUNC_FIX_ARG( language, 2, false, false )
{
	std::string lang, std_ver;
	EVAL_AND_CHECK( "language", args[ 0 ], lang );
	EVAL_AND_CHECK( "language", args[ 1 ], std_ver );
	if( lang == "cpp" ) lang = "c++";

#ifdef __linux__
	Env::SetVar( "CC", lang == "c++" ? "g++" : "gcc" );
#elif __APPLE__
	Env::SetVar( "CC", lang == "c++" ? "clang++" : "clang" );
#endif

	Env::SetVar( "LANGUAGE", lang );
	Env::SetVar( "STD_VERSION", std_ver );
	return OK;
}

AL_FUNC_FIX_ARG( license, 1, false, false )
{
	std::string license;
	EVAL_AND_CHECK( "license", args[ 0 ], license );
	Env::SetVar( "LICENSE", license );
	return OK;
}

AL_FUNC_FIX_ARG( author, 2, false, false )
{
	std::string name, email;
	EVAL_AND_CHECK( "author", args[ 0 ], name );
	EVAL_AND_CHECK( "author", args[ 0 ], email );

	Env::SetVar( "AUTHOR_NAME", name );
	Env::SetVar( "AUTHOR_EMAIL", email );
	return OK;
}