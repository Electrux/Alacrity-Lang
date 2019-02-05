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
#include "../../../include/Interpreter.hpp"
#include "../../../include/Interpreter/Block.hpp"
#include "../../../include/Interpreter/FnBase.hpp"

AL_FUNC_VAR_ARG( add_cxx_flags, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "add_cxx_flags", * arg, farg );
		Env::Append( "CXX_FLAGS", farg, ' ' );
	}
	return OK;
}

AL_FUNC_VAR_ARG( add_cxx_inc_dirs, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "add_cxx_flags", * arg, farg );
		Env::Append( "CXX_INC_DIRS", farg, ' ' );
	}
	return OK;
}

AL_FUNC_VAR_ARG( add_cxx_lib_dirs, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "add_cxx_flags", * arg, farg );
		Env::Append( "CXX_LIB_DIRS", farg, ' ' );
	}
	return OK;
}

AL_FUNC_VAR_ARG( add_cxx_lib_flags, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		std::string farg;
		EVAL_AND_CHECK( "add_cxx_flags", * arg, farg );
		Env::Append( "CXX_LIB_FLAGS", farg, ' ' );
	}
	return OK;
}