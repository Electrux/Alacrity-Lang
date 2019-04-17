#include <ctime>
#include <chrono>

#include "../../include/Errors.hpp"
#include "../../include/Env.hpp"
#include "../../include/Interpreter/FnBase.hpp"

AL_FUNC_FIX_ARG( get, 1, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "get", args[ 0 ], var );
	auto time_now = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
	Env::SetVar( var, ctime( & time_now ) );
	return OK;
}