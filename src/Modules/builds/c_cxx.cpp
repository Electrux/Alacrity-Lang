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
#include "../../../include/FS.hpp"
#include "../../../include/IO.hpp"
#include "../../../include/Interpreter.hpp"
#include "../../../include/Interpreter/Block.hpp"
#include "../../../include/Interpreter/FnBase.hpp"

static bool CheckCompilerFlag( const std::string & flag );

AL_FUNC_VAR_ARG( add_flags, 1, -1, false, false )
{
	std::string loc = Env::GetVar( "LANGUAGE" ) == "c++" ? "CXX_FLAGS" : "C_FLAGS";
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "add_flags", * arg, farg );
		if( farg.size() > 0 && CheckCompilerFlag( farg ) ) Env::Append( loc, farg, ' ' );
	}
	return OK;
}

AL_FUNC_VAR_ARG( add_inc_dirs, 1, -1, false, false )
{
	std::string loc = Env::GetVar( "LANGUAGE" ) == "c++" ? "CXX_INC_DIRS" : "C_INC_DIRS";
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "add_inc_dirs", * arg, farg );
		Env::Append( loc, farg, ' ' );
	}
	return OK;
}

AL_FUNC_VAR_ARG( add_lib_dirs, 1, -1, false, false )
{
	std::string loc = Env::GetVar( "LANGUAGE" ) == "c++" ? "CXX_LIB_DIRS" : "C_LIB_DIRS";
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "add_lib_dirs", * arg, farg );
		Env::Append( loc, farg, ' ' );
	}
	return OK;
}

AL_FUNC_VAR_ARG( add_lib_flags, 1, -1, false, false )
{
	std::string loc = Env::GetVar( "LANGUAGE" ) == "c++" ? "CXX_LIB_FLAGS" : "C_LIB_FLAGS";
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "add_lib_flags", * arg, farg );
		Env::Append( loc, farg, ' ' );
	}
	return OK;
}

static bool CheckCompilerFlag( const std::string & flag )
{
	// skip the -D flag
	if( flag.size() > 1 && flag[ 1 ] == 'D' ) return true;
	IO::colout << "{b}Checking if compiler supports " << flag << " {0}... ";
	std::cout.flush();
	std::string cmd = "echo \"#include <stdio.h>\\nint main() \\{ return 0; }\" | ${CC} " + flag
		+ " -o /dev/null -x" + Env::GetVar( "LANGUAGE" ) + " - 2>/dev/null";
	std::string fcmd;
	EVAL_AND_CHECK( "CheckCompilerFlag", cmd, fcmd );
	bool res = Env::Exec( fcmd ) == 0;
	IO::colout << ( res ? "{g}yes{0}\n" : "{r}no{0}\n" );
	return res;
}