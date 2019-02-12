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
#include "../../include/Env.hpp"
#include "../../include/String.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

AL_FUNC_FIX_ARG( each, 2, true, true )
{
	if( block == nullptr ) return OK;
	std::string var;
	EVAL_AND_CHECK( "each", args[ 1 ], var );

	std::string loop_var;
	EVAL_AND_CHECK( "each", args[ 0 ], loop_var );
	int res = OK;
	for( auto & e : var ) {
		Env::SetVar( loop_var, std::string( 1, e ) );
		res = Interpreter::Block( block, depth, internal_display_enabled );
		if( res != OK && res != LOOP_CONTINUE_ENCOUNTERED ) break;
	}
	if( res == LOOP_BREAK_ENCOUNTERED || res == LOOP_CONTINUE_ENCOUNTERED ) res = OK;
	Env::Reset( loop_var );
	return res;
}

AL_FUNC_FIX_ARG( each_idx, 3, true, true )
{
	if( block == nullptr ) return OK;
	std::string var;
	EVAL_AND_CHECK( "each_idx", args[ 2 ], var );

	std::string loop_var, loop_char;
	EVAL_AND_CHECK( "each_idx", args[ 0 ], loop_var );
	EVAL_AND_CHECK( "each_idx", args[ 1 ], loop_char );
	int res = OK;
	for( int i = 0; i < ( int )var.size(); ++i ) {
		Env::SetVar( loop_var, std::to_string( i ) );
		Env::SetVar( loop_char, std::string( 1, var[ i ] ) );
		res = Interpreter::Block( block, depth, internal_display_enabled );
		if( res != OK && res != LOOP_CONTINUE_ENCOUNTERED ) break;
	}
	if( res == LOOP_BREAK_ENCOUNTERED || res == LOOP_CONTINUE_ENCOUNTERED ) res = OK;
	Env::Reset( loop_var );
	Env::Reset( loop_char );
	return res;
}

AL_FUNC_FIX_ARG( add, 3, false, false )
{
	std::string val = Env::GetVar( args[ 0 ] );
	std::string idx_str;
	EVAL_AND_CHECK( "add", args[ 1 ], idx_str );
	CHECK_VAR_NUMERIC( args[ 1 ], idx_str );
	std::string add_val_str;
	EVAL_AND_CHECK( "add", args[ 2 ], add_val_str );
	CHECK_VAR_NUMERIC( args[ 2 ], add_val_str );
	size_t idx = std::stoul( idx_str );
	int add_val = std::stoi( add_val_str );
	if( idx >= val.size() ) {
		std::cerr << "libstring: Provided index: " << idx_str << " does not fall in the range [0, " << std::to_string( val.size() ) << "]\n";
		return FAIL;
	}

	val[ idx ] += add_val;
	Env::SetVar( args[ 0 ], val );
	return OK;
}