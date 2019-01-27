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
#include "../../include/IO.hpp"
#include "../../include/String.hpp"
#include "../../include/Env.hpp"
#include "../../include/FS.hpp"
#include "../../include/Lexer/Lexer.hpp"
#include "../../include/Parser.hpp"
#include "../../include/Interpreter.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

AL_FUNC_FIX_ARG( fail, 1, false, false )
{
	std::string op;
	int res = OK;
	EVAL_AND_CHECK( "fail", res, "{br}FAIL{0}: {r}" + args[ 0 ] + "{0}", op );
	Str::RemoveBackslash( op );
	std::cout << op;
	std::cout.flush();
	return FAIL_FN_CALLED;
}

AL_FUNC_VAR_ARG( print, 1, -1, false, false )
{
	int res = OK;
	for( auto & a : args ) {
		std::string op;
		EVAL_AND_CHECK( "print", res, a, op );
		Str::RemoveBackslash( op );
		std::cout << op;
	}
	std::cout.flush();
	return res;
}

AL_FUNC_FIX_ARG( input, 2, false, false )
{
	int res = OK;
	std::string op;
	EVAL_AND_CHECK( "input", res, args[ 1 ], op );
	Str::RemoveBackslash( op );
	std::cout << op;
	std::cout.flush();
	std::string tmp;
	std::cin.ignore( std::numeric_limits< std::streamsize >::max(), '\n' );
	std::getline( std::cin, tmp );
	Env::SetVar( args[ 0 ], tmp );
	return res;
}

AL_FUNC_FIX_ARG( input_num, 2, false, false )
{
	int res = OK;
	std::string op;
	EVAL_AND_CHECK( "input_num", res, args[ 1 ], op );
	Str::RemoveBackslash( op );
	std::string tmp;
	std::cout << op;
	std::cout.flush();
	std::cin.ignore( std::numeric_limits< std::streamsize >::max(), '\n' );
	while( tmp.empty() ) {
		std::getline( std::cin, tmp );
		if( !Str::IsNum( tmp ) ) {
			std::cout << "Input wasn't a number! Try again: ";
			std::cout.flush();
			tmp.clear();
		}
	}
	Env::SetVar( args[ 0 ], tmp );
	return res;
}

AL_FUNC_FIX_ARG( set, 2, false, false )
{
	if( args[ 0 ].empty() ) return OK;
	std::string op1, op2;
	int res = OK;
	EVAL_AND_CHECK( "set", res, args[ 0 ], op1 );
	EVAL_AND_CHECK( "set", res, args[ 1 ], op2 );
	if( !Env::SetVar( op1, op2 ) ) {
		std::cout << "Function: set failed to set the variable: " << op1
			<< " to: " << op2 << "\n";
		return ENV_SETVAR_FAILED;
	}
	return res;
}

AL_FUNC_VAR_ARG( prepend, 2, -1, false, false )
{
	std::string var = args[ 0 ];
	int res = OK;
	for( auto arg = args.begin() + 1; arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "prepend", res, * arg, farg );
		Env::Prepend( var, farg );
	}
	return res;
}

AL_FUNC_VAR_ARG( append, 2, -1, false, false )
{
	std::string var = args[ 0 ];
	int res = OK;
	for( auto arg = args.begin() + 1; arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "append", res, * arg, farg );
		Env::Append( var, farg );
	}
	return res;
}

AL_FUNC_VAR_ARG( remove, 2, -1, false, false )
{
	std::string var = args[ 0 ];
	int res = OK;
	for( auto arg = args.begin() + 1; arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "remove", res, * arg, farg );
		Env::Remove( var, farg );
	}
	return res;
}

AL_FUNC_FIX_ARG( reset, 1, false, false )
{
	Env::Reset( args[ 0 ] );
	return OK;
}

AL_FUNC_FIX_ARG( exec, 1, false, false )
{
	int res = OK;
	std::string op;
	EVAL_AND_CHECK( "exec", res, args[ 0 ], op );
	Env::Exec( op, nullptr );
	return res;
}

AL_FUNC_FIX_ARG( eval, 1, false, false )
{
	std::string tmp;
	Str::Eval( args[ 0 ], tmp );
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