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

#include "../../../../include/Errors.hpp"
#include "../../../../include/Core.hpp"
#include "../../../../include/String.hpp"
#include "../../../../include/Env.hpp"
#include "../../../../include/FS.hpp"
#include "../../../../include/Lexer/Lexer.hpp"
#include "../../../../include/Parser.hpp"
#include "../../../../include/Interpreter.hpp"
#include "../../../../include/Interpreter/FnBase.hpp"

EBS_FUNC_VAR_ARG( add_c_flags, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		Env::Append( "C_FLAGS", * arg, ' ' );
	}
	return OK;
}

EBS_FUNC_VAR_ARG( add_c_inc_dirs, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		Env::Append( "C_INCLUDE_DIRS", Env::AddCurrDir( * arg ), ' ' );
	}
	return OK;
}

EBS_FUNC_VAR_ARG( add_c_lib_dirs, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		Env::Append( "C_LIB_DIRS", Env::AddCurrDir( * arg ), ' ' );
	}
	return OK;
}

EBS_FUNC_VAR_ARG( add_c_lib_flags, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		Env::Append( "C_LIB_FLAGS", * arg, ' ' );
	}
	return OK;
}

// Remove equivalents

EBS_FUNC_VAR_ARG( rem_c_flags, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		if( Env::GetVar( "C_FLAGS" ).empty() ) return OK;
		Env::Remove( "C_FLAGS", * arg, ' ' );
	}
	return OK;
}

EBS_FUNC_VAR_ARG( rem_c_lib_flags, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		if( Env::GetVar( "C_LIB_FLAGS" ).empty() ) return OK;
		Env::Remove( "C_LIB_FLAGS", * arg, ' ' );
	}
	return OK;
}