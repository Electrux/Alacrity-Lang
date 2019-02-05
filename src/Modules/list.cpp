#include <iostream>
#include <vector>
#include <string>

#include "../../include/Errors.hpp"
#include "../../include/Core.hpp"
#include "../../include/IO.hpp"
#include "../../include/String.hpp"
#include "../../include/Env.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

AL_FUNC_FIX_ARG( create, 1, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "create", args[ 0 ], var );
	std::vector< std::string > * vec = new std::vector< std::string >();
	auto loc = std::to_string( ( unsigned long long )vec );
	if( vec == nullptr ) {
		std::cerr << "liblist: Failed to create std::vector to location: " << loc << "\n";
		return FAIL;
	}
	Env::SetVar( var, loc );
	return OK;
}

AL_FUNC_FIX_ARG( destroy, 1, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "destroy", args[ 0 ], var );
	auto loc = Env::GetVar( var );
	std::vector< std::string > * vec = ( std::vector< std::string > * )std::stoull( loc );
	if( vec == nullptr ) {
		std::cerr << "liblist: Failed to fetch std::vector from location: " << loc << "\n";
		return FAIL;
	}
	delete vec;
	return OK;
}

AL_FUNC_FIX_ARG( push, 2, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "push", args[ 0 ], var );
	auto loc = Env::GetVar( var );
	std::vector< std::string > * vec = ( std::vector< std::string > * )std::stoull( loc );
	if( vec == nullptr ) {
		std::cerr << "liblist: Failed to fetch std::vector from location: " << loc << "\n";
		return FAIL;
	}
	vec->push_back( args[ 1 ] );
	return OK;
}

AL_FUNC_VAR_ARG( pop, 1, 2, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "pop", args[ 0 ], var );
	auto loc = Env::GetVar( var );
	std::vector< std::string > * vec = ( std::vector< std::string > * )std::stoull( loc );
	if( vec == nullptr ) {
		std::cerr << "liblist: Failed to fetch std::vector from location: " << loc << "\n";
		return FAIL;
	}
	if( vec->empty() ) return OK;
	std::string res_loc = "RESULT";
	if( args.size() > 1 ) {
		EVAL_AND_CHECK( "pop", args[ 1 ], res_loc );
	}
	Env::SetVar( res_loc, * ( vec->end() - 1 ) );
	vec->pop_back();
	return OK;
}

AL_FUNC_VAR_ARG( erase, 2, 3, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "erase", args[ 0 ], var );
	auto loc = Env::GetVar( var );
	std::vector< std::string > * vec = ( std::vector< std::string > * )std::stoull( loc );
	if( vec == nullptr ) {
		std::cerr << "liblist: Failed to fetch std::vector from location: " << loc << "\n";
		return FAIL;
	}
	if( vec->empty() ) return OK;

	std::string idx;
	EVAL_AND_CHECK( "erase", args[ 1 ], idx );
	CHECK_VAR_NUMERIC( args[ 1 ], idx );
	size_t index = std::stoul( idx );
	if( vec->size() <= index ) return OK;

	std::string res_loc = "RESULT";
	if( args.size() > 2 ) {
		EVAL_AND_CHECK( "erase", args[ 2 ], res_loc );
	}
	Env::SetVar( res_loc, ( * vec )[ index ] );
	vec->erase( vec->begin() + index );
	return OK;
}

AL_FUNC_VAR_ARG( at, 2, 3, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "at", args[ 0 ], var );
	auto loc = Env::GetVar( var );
	std::vector< std::string > * vec = ( std::vector< std::string > * )std::stoull( loc );
	if( vec == nullptr ) {
		std::cerr << "liblist: Failed to fetch std::vector from location: " << loc << "\n";
		return FAIL;
	}
	if( vec->empty() ) return OK;

	std::string idx;
	EVAL_AND_CHECK( "pop", args[ 1 ], idx );
	CHECK_VAR_NUMERIC( args[ 1 ], idx );
	size_t index = std::stoul( idx );
	if( vec->size() <= index ) return OK;

	std::string res_loc = "RESULT";
	if( args.size() > 2 ) {
		EVAL_AND_CHECK( "at", args[ 2 ], res_loc );
	}
	Env::SetVar( res_loc, ( * vec )[ index ] );
	return OK;
}

AL_FUNC_FIX_ARG( each, 2, true, true )
{
	if( block == nullptr ) return OK;
	std::string var;
	EVAL_AND_CHECK( "each", args[ 0 ], var );
	auto loc = Env::GetVar( var );
	std::vector< std::string > * vec = ( std::vector< std::string > * )std::stoull( loc );
	if( vec == nullptr ) {
		std::cerr << "liblist: Failed to fetch std::vector from location: " << loc << "\n";
		return FAIL;
	}
	if( vec->empty() ) return OK;

	std::string loop_var;
	EVAL_AND_CHECK( "each", args[ 1 ], loop_var );
	int res = OK;
	for( auto & e : * vec ) {
		Env::SetVar( loop_var, e );
		res = Interpreter::Block( block, depth, internal_display_enabled );
		if( res != OK && res != LOOP_CONTINUE_ENCOUNTERED ) break;
	}
	if( res == LOOP_BREAK_ENCOUNTERED || res == LOOP_CONTINUE_ENCOUNTERED ) res = OK;
	Env::Reset( loop_var );
	return res;
}

AL_FUNC_FIX_ARG( clear, 1, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "clear", args[ 0 ], var );
	auto loc = Env::GetVar( var );
	std::vector< std::string > * vec = ( std::vector< std::string > * )std::stoull( loc );
	if( vec == nullptr ) {
		std::cerr << "liblist: Failed to fetch std::vector from location: " << loc << "\n";
		return FAIL;
	}
	vec->clear();
	return OK;
}