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
#include <limits>

#include "../../include/Errors.hpp"
#include "../../include/Core.hpp"
#include "../../include/String.hpp"
#include "../../include/Env.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

AL_FUNC_FIX_ARG( fail, 1, false, false )
{
	std::string op;
	EVAL_AND_CHECK( "fail", "{br}FAIL{0}: {r}" + args[ 0 ] + "{0}", op );
	std::cout << op;
	std::cout.flush();
	return FAIL_FN_CALLED;
}

AL_FUNC_VAR_ARG( print, 1, -1, false, false )
{
	for( auto & a : args ) {
		std::string op;
		EVAL_AND_CHECK( "print", a, op );
		std::cout << op;
	}
	std::cout.flush();
	return OK;
}

AL_FUNC_FIX_ARG( input, 2, false, false )
{
	std::string op;
	EVAL_AND_CHECK( "input", args[ 1 ], op );
	std::cout << op;
	std::cout.flush();
	std::string tmp;
	std::getline( std::cin, tmp );
	Env::SetVar( args[ 0 ], tmp );
	return OK;
}

AL_FUNC_FIX_ARG( input_num, 2, false, false )
{
	std::string op;
	EVAL_AND_CHECK( "input_num", args[ 1 ], op );
	std::string tmp;
	std::cout << op;
	std::cout.flush();
	while( tmp.empty() ) {
		std::getline( std::cin, tmp );
		if( !Str::IsNum( tmp ) ) {
			std::cout << "Input wasn't a number! Try again: ";
			std::cout.flush();
			tmp.clear();
		}
	}
	Env::SetVar( args[ 0 ], tmp );
	return OK;
}

AL_FUNC_FIX_ARG( set, 2, false, false )
{
	if( args[ 0 ].empty() ) return OK;
	std::string op1, op2;
	EVAL_AND_CHECK( "set", args[ 0 ], op1 );
	EVAL_AND_CHECK( "set", args[ 1 ], op2 );
	if( !Env::SetVar( op1, op2 ) ) {
		std::cout << "Function: set failed to set the variable: " << op1
			<< " to: " << op2 << "\n";
		return ENV_SETVAR_FAILED;
	}
	return OK;
}

AL_FUNC_VAR_ARG( prepend, 3, -1, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "prepend", args[ 0 ], var );
	char delim = args[ 1 ].empty() ? ':' : args[ 1 ][ 0 ];
	for( auto arg = args.begin() + 2; arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "prepend", * arg, farg );
		Env::Prepend( var, farg, delim );
	}
	return OK;
}

AL_FUNC_VAR_ARG( append, 3, -1, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "append", args[ 0 ], var );
	char delim = args[ 1 ].empty() ? ':' : args[ 1 ][ 0 ];
	for( auto arg = args.begin() + 2; arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "append", * arg, farg );
		Env::Append( var, farg, delim );
	}
	return OK;
}

AL_FUNC_VAR_ARG( remove, 3, -1, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "remove", args[ 0 ], var );
	char delim = args[ 1 ].empty() ? ':' : args[ 1 ][ 0 ];
	for( auto arg = args.begin() + 2; arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "remove", * arg, farg );
		Env::Remove( var, farg, delim );
	}
	return OK;
}

AL_FUNC_FIX_ARG( reset, 1, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "reset", args[ 0 ], var );
	Env::Reset( var );
	return OK;
}

AL_FUNC_VAR_ARG( bs_add_lib_paths, 1, -1, false, false )
{
	std::string var = Core::ALLibPaths();
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "bs_add_lib_paths", * arg, farg );
		Env::Append( var, farg );
	}
	return OK;
}

AL_FUNC_FIX_ARG( exec, 1, false, false )
{
	std::string op;
	EVAL_AND_CHECK( "exec", args[ 0 ], op );
	Env::Exec( op, nullptr );
	return OK;
}

AL_FUNC_FIX_ARG( eval, 1, false, false )
{
	std::string tmp;
	EVAL_AND_CHECK( "eval", args[ 0 ], tmp );
	return OK;
}

AL_FUNC_FIX_ARG( enable_internal_display, 0, false, false )
{
	return IMPLICIT_DISPLAY_ENABLED;
}

AL_FUNC_FIX_ARG( disable_internal_display, 0, false, false )
{
	return IMPLICIT_DISPLAY_DISABLED;
}

AL_FUNC_FIX_ARG( brkloop, 0, false, false )
{
	return LOOP_BREAK_ENCOUNTERED;
}

AL_FUNC_FIX_ARG( contloop, 0, false, false )
{
	return LOOP_CONTINUE_ENCOUNTERED;
}