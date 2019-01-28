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
#include <cmath>

#include "../../include/Errors.hpp"
#include "../../include/Env.hpp"
#include "../../include/String.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

#define CHECK_VAR_NUMERIC( var, val ) 						\
if( val.empty() || !Str::IsNum( val ) ) {					\
	std::cerr << "Expected variable: " << var << " to contain a number!\n";	\
	return EXPECTED_NUM_VAL;						\
}

AL_FUNC_FIX_ARG( inc, 1, false, false )
{
	std::string op;
	EVAL_AND_CHECK( "inc", args[ 0 ], op );
	auto val = Env::GetVar( op );
	CHECK_VAR_NUMERIC( op, val );

	Env::SetVar( op, std::to_string( std::stoi( val ) + 1 ) );

	return OK;
}


AL_FUNC_FIX_ARG( dec, 1, false, false )
{
	std::string op;
	EVAL_AND_CHECK( "dec", args[ 0 ], op );
	auto val = Env::GetVar( op );
	CHECK_VAR_NUMERIC( op, val );

	Env::SetVar( op, std::to_string( std::stoi( val ) - 1 ) );

	return OK;
}

// Args:
//  when 2: RESULT <- ARG1 + ARG2
//  when 3: ARG1 <- ARG2 + ARG3
AL_FUNC_VAR_ARG( add, 2, 3, false, false )
{
	if( args.size() == 2 ) {
		std::string val1;
		EVAL_AND_CHECK( "add", args[ 0 ], val1 );
		CHECK_VAR_NUMERIC( args[ 0 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( "add", args[ 1 ], val2 );
		CHECK_VAR_NUMERIC( args[ 1 ], val2 );

		Env::SetVar( "RESULT", std::to_string( std::stoi( val1 ) + std::stoi( val2 ) ) );
	} else { // args.size() == 3
		std::string val1;
		EVAL_AND_CHECK( "add", args[ 1 ], val1 );
		CHECK_VAR_NUMERIC( args[ 1 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( "add", args[ 2 ], val2 );
		CHECK_VAR_NUMERIC( args[ 2 ], val2 );

		Env::SetVar( args[ 0 ], std::to_string( std::stoi( val1 ) + std::stoi( val2 ) ) );
	}
	return OK;
}

// Args:
//  when 2: RESULT <- ARG1 - ARG2
//  when 3: ARG1 <- ARG2 - ARG3
AL_FUNC_VAR_ARG( sub, 2, 3, false, false )
{
	if( args.size() == 2 ) {
		std::string val1;
		EVAL_AND_CHECK( "sub", args[ 0 ], val1 );
		CHECK_VAR_NUMERIC( args[ 0 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( "sub", args[ 1 ], val2 );
		CHECK_VAR_NUMERIC( args[ 1 ], val2 );

		Env::SetVar( "RESULT", std::to_string( std::stoi( val1 ) - std::stoi( val2 ) ) );
	} else { // args.size() == 3
		std::string val1;
		EVAL_AND_CHECK( "sub", args[ 1 ], val1 );
		CHECK_VAR_NUMERIC( args[ 1 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( "sub", args[ 2 ], val2 );
		CHECK_VAR_NUMERIC( args[ 2 ], val2 );

		Env::SetVar( args[ 0 ], std::to_string( std::stoi( val1 ) - std::stoi( val2 ) ) );
	}
	return OK;
}

// Args:
//  when 2: RESULT <- ARG1 * ARG2
//  when 3: ARG1 <- ARG2 * ARG3
AL_FUNC_VAR_ARG( mul, 2, 3, false, false )
{
	if( args.size() == 2 ) {
		std::string val1;
		EVAL_AND_CHECK( "mul", args[ 0 ], val1 );
		CHECK_VAR_NUMERIC( args[ 0 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( "mul", args[ 1 ], val2 );
		CHECK_VAR_NUMERIC( args[ 1 ], val2 );

		Env::SetVar( "RESULT", std::to_string( std::stoi( val1 ) * std::stoi( val2 ) ) );
	} else { // args.size() == 3
		std::string val1;
		EVAL_AND_CHECK( "mul", args[ 1 ], val1 );
		CHECK_VAR_NUMERIC( args[ 1 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( "mul", args[ 2 ], val2 );
		CHECK_VAR_NUMERIC( args[ 2 ], val2 );

		Env::SetVar( args[ 0 ], std::to_string( std::stoi( val1 ) * std::stoi( val2 ) ) );
	}
	return OK;
}

// Args:
//  when 2: RESULT <- ARG1 / ARG2
//  when 3: ARG1 <- ARG2 / ARG3
AL_FUNC_VAR_ARG( div, 2, 3, false, false )
{
	if( args.size() == 2 ) {
		std::string val1;
		EVAL_AND_CHECK( "div", args[ 0 ], val1 );
		CHECK_VAR_NUMERIC( args[ 0 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( "div", args[ 1 ], val2 );
		CHECK_VAR_NUMERIC( args[ 1 ], val2 );

		Env::SetVar( "RESULT", std::to_string( std::stoi( val1 ) / std::stoi( val2 ) ) );
	} else { // args.size() == 3
		std::string val1;
		EVAL_AND_CHECK( "div", args[ 1 ], val1 );
		CHECK_VAR_NUMERIC( args[ 1 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( "div", args[ 2 ], val2 );
		CHECK_VAR_NUMERIC( args[ 2 ], val2 );

		Env::SetVar( args[ 0 ], std::to_string( std::stoi( val1 ) / std::stoi( val2 ) ) );
	}
	return OK;
}

// Args:
//  when 2: RESULT <- ARG1 % ARG2
//  when 3: ARG1 <- ARG2 % ARG3
AL_FUNC_VAR_ARG( mod, 2, 3, false, false )
{
	if( args.size() == 2 ) {
		std::string val1;
		EVAL_AND_CHECK( "mod", args[ 0 ], val1 );
		CHECK_VAR_NUMERIC( args[ 0 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( "mod", args[ 1 ], val2 );
		CHECK_VAR_NUMERIC( args[ 1 ], val2 );

		Env::SetVar( "RESULT", std::to_string( std::stoi( val1 ) % std::stoi( val2 ) ) );
	} else { // args.size() == 3
		std::string val1;
		EVAL_AND_CHECK( "mod", args[ 1 ], val1 );
		CHECK_VAR_NUMERIC( args[ 1 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( "mod", args[ 2 ], val2 );
		CHECK_VAR_NUMERIC( args[ 2 ], val2 );

		Env::SetVar( args[ 0 ], std::to_string( std::stoi( val1 ) % std::stoi( val2 ) ) );
	}
	return OK;
}

// Args:
//  when 2: RESULT <- sqrt( ARG1 )
//  when 3: ARG1 <- sqrt( ARG2 )
AL_FUNC_VAR_ARG( sqrt, 1, 2, false, false )
{
	if( args.size() == 1 ) {
		std::string val;
		EVAL_AND_CHECK( "sqrt", args[ 0 ], val );
		CHECK_VAR_NUMERIC( args[ 0 ], val );

		Env::SetVar( "RESULT", std::to_string( ( int )std::sqrt( std::stoi( val ) ) ) );
	} else { // args.size() == 2
		std::string val;
		EVAL_AND_CHECK( "sqrt", args[ 1 ], val );
		CHECK_VAR_NUMERIC( args[ 1 ], val );

		Env::SetVar( args[ 0 ], std::to_string( ( int )std::sqrt( std::stoi( val ) ) ) );
	}
	return OK;
}

AL_FUNC_VAR_ARG( isprime, 1, 2, false, false )
{
	int arg_loc = args.size() == 1 ? 0 : 1;
	std::string var = args.size() > 1 ? args[ 0 ] : "RESULT";
	std::string val;
	EVAL_AND_CHECK( "isprime", args[ arg_loc ], val );
	CHECK_VAR_NUMERIC( args[ arg_loc ], val );

	int num = std::stoi( val );
	int sqroot = std::sqrt( num );
	bool is_prime = true;
	for( int i = 2; i <= sqroot; ++i ) {
		if( num % i == 0 ) { is_prime = false; break; }
	}

	Env::SetVar( var, is_prime ? "true" : "false" );
	return OK;
}