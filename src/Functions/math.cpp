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
#include "../../include/Env.hpp"
#include "../../include/String.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

#define CHECK_VAR_NUMERIC( var, val ) 						\
if( val.empty() || !Str::IsNum( val ) ) {					\
	std::cerr << "Expected variable: " << var << " to contain a number!\n";	\
	return EXPECTED_NUM_VAL;						\
}

AL_FUNC_FIX_ARG( inc, 2, false, false )
{
	int res = OK;
	std::string op;
	EVAL_AND_CHECK( "inc", res, args[ 0 ], op );
	auto val = Env::GetVar( op );
	CHECK_VAR_NUMERIC( op, val );

	Env::SetVar( op, std::to_string( std::stoi( val ) + 1 ) );

	return res;
}


AL_FUNC_FIX_ARG( dec, 2, false, false )
{
	int res = OK;
	std::string op;
	EVAL_AND_CHECK( "dec", res, args[ 0 ], op );
	auto val = Env::GetVar( op );
	CHECK_VAR_NUMERIC( op, val );

	Env::SetVar( op, std::to_string( std::stoi( val ) - 1 ) );

	return res;
}

/*
// Args:
//  when 2: RESULT <- ARG1 + ARG2
//  when 3: ARG1 <- ARG2 + ARG3
AL_FUNC_VAR_ARG( add, 2, 3, false, false )
{
	if( args.size() == 2 ) {
		std::string val1;
		EVAL_AND_CHECK( args[ 0 ], val1 );
		CHECK_VAR_NUMERIC( args[ 0 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( args[ 1 ], val2 );
		CHECK_VAR_NUMERIC( args[ 1 ], val2 );

		Env::SetVar( "RESULT", std::to_string( std::stoi( val1 ) + std::stoi( val2 ) ) );
	} else { // args.size() == 3
		std::string val1;
		EVAL_AND_CHECK( args[ 1 ], val1 );
		CHECK_VAR_NUMERIC( args[ 1 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( args[ 2 ], val2 );
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
		EVAL_AND_CHECK( args[ 0 ], val1 );
		CHECK_VAR_NUMERIC( args[ 0 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( args[ 1 ], val2 );
		CHECK_VAR_NUMERIC( args[ 1 ], val2 );

		Env::SetVar( "RESULT", std::to_string( std::stoi( val1 ) - std::stoi( val2 ) ) );
	} else { // args.size() == 3
		std::string val1;
		EVAL_AND_CHECK( args[ 1 ], val1 );
		CHECK_VAR_NUMERIC( args[ 1 ], val1 );
		std::string val2;
		EVAL_AND_CHECK( args[ 2 ], val2 );
		CHECK_VAR_NUMERIC( args[ 2 ], val2 );

		Env::SetVar( args[ 0 ], std::to_string( std::stoi( val1 ) - std::stoi( val2 ) ) );
	}
	return OK;
}
*/