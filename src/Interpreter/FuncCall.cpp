/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <iostream>

#include "../../include/Errors.hpp"
#include "../../include/Env.hpp"
#include "../../include/Core.hpp"
#include "../../include/IO.hpp"
#include "../../include/DynLib.hpp"
#include "../../include/Parser.hpp"
#include "../../include/Parser/BlockStmt.hpp"
#include "../../include/Interpreter/FnBase.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FuncCall.hpp"

static int GetFuncFileLoc( const std::string & fn_name, const std::string & fn_lib_name, const int name_vec_size,
			std::string & file_loc, const int depth );
static bool VerifyArgCount( const std::string & fn_name, const int depth, const int min, const int max, const int used );

int Interpreter::FuncCall( const Parser::FnCallStmt * fncall, const size_t depth )
{
	int res = OK;
	std::string err;

	std::string file_loc;
	res = GetFuncFileLoc( fncall->GetFullName(), fncall->GetLibName(), fncall->GetNameVec().size(), file_loc, depth );
	if( res != OK && res != USING_CORE_LIB ) return res;

	auto handle = DynLib::Get()->Load( file_loc );
	FnInfo * func_info = nullptr;
	int ( * func )( const std::vector< std::string > &, const int, const Parser::BlockStmt *, const bool ) = nullptr;

	if( handle == nullptr ) {
		err = "dlopen() failed for: " + file_loc;
		res = DLOPEN_FAILED;
		goto error;
	}

	func_info = ( FnInfo * )DynLib::Get()->GetSym( file_loc, "f_inf_" + fncall->GetName() );

	if( func_info == nullptr ) {
		if( res == USING_CORE_LIB ) {
			err = "dlsym() failed for: f_inf (in core library, since there exists no library file of this name)";
		} else {
			err = "dlsym() failed for: f_inf";
		}
		res = DLSYM_FAILED;
		goto error;
	}
	if( !VerifyArgCount( fncall->GetName(), depth, func_info->min_args, func_info->max_args, fncall->GetArgs().size() ) ) {
		res = ARG_COUNT_MISMATCH;
		goto error;
	}
	if( !func_info->persist_env && fncall->GetBlock() != nullptr ) {
		Env::Backup();
	}

	func = ( int (*)( const std::vector< std::string > &, const int, const Parser::BlockStmt *, const bool ) )
		DynLib::Get()->GetSym( file_loc, "fn_" + fncall->GetName() );
	if( func == nullptr ) {
		err = "dlsym() failed for: " + fncall->GetName();
		res = DLSYM_FAILED;
		goto error;
	}

	// execute the function
	res = func( fncall->GetArgs(), depth, fncall->GetBlock(), IO::colout.IsEnabled() );

	if( res == COLOR_OUT_ENABLED || res == COLOR_OUT_DISABLED ) {
		IO::colout.UseColors( res == COLOR_OUT_ENABLED );
		res = OK;
	}

	if( res == IMPLICIT_DISPLAY_ENABLED || res == IMPLICIT_DISPLAY_DISABLED ) {
		IO::colout.Enable( res == IMPLICIT_DISPLAY_ENABLED );
		res = OK;
	}

	if( res != OK && res != LOOP_BREAK_ENCOUNTERED && res != LOOP_CONTINUE_ENCOUNTERED ) {
		if( res != FAIL_FN_CALLED ) {
			err = "Check the error above";
		}
		goto error;
	}

	if( fncall->GetBlock() != nullptr ) {
		if( !func_info->used_block && res != LOOP_BREAK_ENCOUNTERED && res != LOOP_CONTINUE_ENCOUNTERED )
			res = Interpreter::Block( fncall->GetBlock(), depth );
		if( !func_info->persist_env ) Env::Restore();
	}
error:
	if( res != OK && res != FAIL_FN_CALLED && res != LOOP_BREAK_ENCOUNTERED && res != LOOP_CONTINUE_ENCOUNTERED ) {
		bool was_enabled = IO::colout.IsEnabled();
		if( !was_enabled ) IO::colout.Enable( true );
		IO::colout << "Function Call[" << depth << "][{b}" << fncall->GetFullName()
			<< "{0}] {r}error{0}(" << res << "): Failed to interpret, " + err + "!{0}\n";
		if( !was_enabled ) IO::colout.Enable( false );
	}
	return res;
}

static int GetFuncFileLoc( const std::string & fn_name, const std::string & fn_lib_name, const int name_vec_size,
			std::string & file_loc, const int depth )
{
	int res = OK;
	std::string err;
	std::string func_search_paths = Env::GetVar( Core::ALLibPaths() );
	if( func_search_paths.empty() ) {
		err = "The env variable: " + Core::ALLibPaths() + " is empty";
		res = ENV_VAR_EMPTY;
		goto error;
	}

	file_loc = Env::GetFileLocation( func_search_paths, fn_lib_name, Core::PathDelim );
	if( !file_loc.empty() ) return OK;

	if( file_loc.empty() && name_vec_size > 1 ) {
		err = "No function library file: " + fn_lib_name + " found for this function in paths: " + func_search_paths;
		res = FN_LIB_NOT_FOUND;
		goto error;
	}
	file_loc = Env::GetFileLocation( func_search_paths, Core::FuncLibFile( "core" ), Core::PathDelim );
	if( file_loc.empty() ) {
		err = "No function library file: " + Core::FuncLibFile( "core" ) + " found for this function in paths: "
			+ func_search_paths + " and the core library isn't available as well!{0}\n";
		res = FN_LIB_NOT_FOUND;
		goto error;
	}
error:
	if( res != OK && res != FAIL_FN_CALLED ) {
		bool was_enabled = IO::colout.IsEnabled();
		if( !was_enabled ) IO::colout.Enable( true );
		IO::colout << "Function Call[" << depth << "][{b}" << fn_name << "{0}] {r}error{0}(" << res << "): Failed to interpret, " + err + "!{0}\n";
		if( !was_enabled ) IO::colout.Enable( false );
		return res;
	}
	return USING_CORE_LIB;
}

static bool VerifyArgCount( const std::string & fn_name, const int depth, const int min, const int max, const int used )
{
	bool res = true;
	std::string err;

	if( min == max && used != min ) {
		err = "Expected arguments " + std::to_string( min ) + ", but found: " + std::to_string( used );
		res = false;
	} else if( min < max && ( used < min || used > max ) ) {
		err = "Expected arguments in range: [" + std::to_string( min ) + ", " + std::to_string( max ) + "], but found: "
			+ std::to_string( used );
		res = false;
	} else if( min > max && used < min ) {
		err = "Expected arguments in range: [" + std::to_string( min ) + ", ...], but found: " + std::to_string( used );
		res = false;
	}

	if( res != true ) {
		bool was_enabled = IO::colout.IsEnabled();
		if( !was_enabled ) IO::colout.Enable( true );
		IO::colout << "Function Call[" << depth << "][{b}" << fn_name << "{0}] {r}error{0}(" << res << "): Failed to interpret, " + err + "!{0}\n";
		if( !was_enabled ) IO::colout.Enable( false );
	}
	return res;
}