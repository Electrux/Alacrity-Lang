/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>
#include <string>

/* Various types of errors */
enum Errors
{
	OK,
	FAIL,

	LOOP_BREAK_ENCOUNTERED,
	LOOP_CONTINUE_ENCOUNTERED,

	UNEXPECTED_TOKEN,
	TOKENS_EMPTY,
	EARLY_EOF,
	ASSN_PARSE_FAIL,
	FN_PARSE_FAIL,
	FN_LIB_NOT_FOUND,
	POPEN_FAILED,
	UNKNOWN_OBJ_TYPE,
	INVALID_STR,
	IDX_OUT_BOUNDS,
	ENV_VAR_EMPTY,
	DLOPEN_FAILED,
	DLSYM_FAILED,
	FAIL_FN_CALLED,
	ARG_COUNT_MISMATCH,
	INVALID_ARG_COUNT,
	FILE_NOT_FOUND,
	DIR_CREATE_FAILED,

	ENV_SETVAR_FAILED,
	ENV_EXEC_FAILED,

	USING_CORE_LIB,

	// Builder errors
	BUILDS_INVALID_TYPE,
	BUILDS_LIB_TYPE_WHEN_NON_LIB,

	EXPECTED_NUM_VAL,
};

inline void err( const std::string & msg, const int line = -1, const int col = -1 )
{
	if( line == -1 ) std::cerr << "Error: " << msg << "\n";
	else std::cerr << "Error on line " << line << " [" << col << "]" << ": " << msg << "\n";
}

inline int ret_err( const std::string & msg, const int err_code = -1, const int line = -1, const int col = -1 )
{
	err( msg, line, col );
	return err_code;
}

#endif // ERRORS_HPP