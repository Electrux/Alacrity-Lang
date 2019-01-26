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
#include "../../../include/Core.hpp"
#include "../../../include/IO.hpp"
#include "../../../include/String.hpp"
#include "../../../include/Env.hpp"
#include "../../../include/FS.hpp"
#include "../../../include/Lexer/Lexer.hpp"
#include "../../../include/Parser.hpp"
#include "../../../include/Interpreter.hpp"
#include "../../../include/Interpreter/Block.hpp"
#include "../../../include/Interpreter/FnBase.hpp"

EBS_FUNC_FIX_ARG( fail, 1, false, false )
{
	std::string op;
	int res = Str::Eval( "{br}FAIL{0}: {r}" + args[ 0 ] + "{0}", op );
	if( res != OK ) {
		std::cout << "Function: fail failed while evaluating: " << op << "\n";
		return res;
	}
	Str::RemoveBackslash( op );
	std::cout << op;
	std::cout.flush();
	return FAIL_FN_CALLED;
}

EBS_FUNC_VAR_ARG( print, 1, -1, false, false )
{
	int res = OK;
	for( auto & a : args ) {
		std::string op;
		res = Str::Eval( a, op );
		if( res != OK ) {
			std::cout << "Function: print failed while evaluating: " << a << "\n";
			return res;
		}
		Str::RemoveBackslash( op );
		std::cout << op;
	}
	std::cout.flush();
	return res;
}

EBS_FUNC_FIX_ARG( set, 2, false, false )
{
	if( args[ 0 ].empty() ) return OK;
	std::string op1, op2;
	int res = OK;
	res = Str::Eval( args[ 0 ], op1 );
	if( res != OK ) {
		std::cout << "Function: set failed while evaluating: " << args[ 0 ] << "\n";
		return res;
	}
	res = Str::Eval( args[ 1 ], op2 );
	if( res != OK ) {
		std::cout << "Function: set failed while evaluating: " << args[ 1 ] << "\n";
		return res;
	}
	if( !Env::SetVar( op1, op2 ) ) {
		std::cout << "Function: set failed to set the variable: " << op1
			<< " to: " << op2 << "\n";
		return ENV_SETVAR_FAILED;
	}
	return res;
}

EBS_FUNC_VAR_ARG( prepend, 2, -1, false, false )
{
	std::string var = args[ 0 ];
	for( auto arg = args.begin() + 1; arg != args.end(); ++arg ) {
		Env::Prepend( var, * arg );
	}
	return OK;
}

EBS_FUNC_VAR_ARG( append, 2, -1, false, false )
{
	std::string var = args[ 0 ];
	for( auto arg = args.begin() + 1; arg != args.end(); ++arg ) {
		Env::Append( var, * arg );
	}
	return OK;
}

EBS_FUNC_VAR_ARG( remove, 2, -1, false, false )
{
	std::string var = args[ 0 ];
	for( auto arg = args.begin() + 1; arg != args.end(); ++arg ) {
		Env::Remove( var, * arg );
	}
	return OK;
}

EBS_FUNC_FIX_ARG( reset, 1, false, false )
{
	Env::Reset( args[ 0 ] );
	return OK;
}

EBS_FUNC_VAR_ARG( bs_add_search_paths, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		Env::Prepend( Core::BSSearchPaths(), Env::AddCurrDir( * arg ) );
	}
	return OK;
}

EBS_FUNC_VAR_ARG( use_lib, 1, -1, false, false )
{
	int res = OK;

	for( auto & arg : args ) {
		auto bs_search_paths = Env::GetVar( Core::BSSearchPaths() );
		auto file_loc = Env::GetFileLocation( bs_search_paths, arg + ".ebs", ':' );
		if( file_loc.empty() ) {
			std::cout << "File for library: " << arg << " not found in search paths: " << bs_search_paths << "\n";
			return FILE_NOT_FOUND;
		}
		auto file_data_var = FS::ReadFile( file_loc );
		if( std::holds_alternative< int >( file_data_var ) ) return std::get< int >( file_data_var );
		std::string file_data = std::get< std::string >( file_data_var );

		auto lex_syms_var = Lex::Tokenize( file_data );

		if( std::holds_alternative< int >( lex_syms_var ) ) return std::get< int >( lex_syms_var );
		auto lex_syms = std::get< Lex::Syms >( lex_syms_var );

		auto parse_syms_var = Parser::ParseTokens( lex_syms.GetSyms() );
		if( std::holds_alternative< int >( parse_syms_var ) ) return std::get< int >( parse_syms_var );
		auto parse_syms = std::get< Parser::ParseTree >( parse_syms_var );

		res = Interpreter::Interpret( parse_syms, Env::GetDirPart( file_loc ), Env::GetFilePart( arg + ".ebs" ),
			depth, internal_display_enabled );
		if( res != OK ) break;
	}

	return res;
}

EBS_FUNC_VAR_ARG( add_compile_flags, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		Env::Append( "COMPILE_FLAGS", * arg, ' ' );
	}
	return OK;
}

EBS_FUNC_VAR_ARG( rem_compile_flags, 1, -1, false, false )
{
	for( auto arg = args.begin(); arg != args.end(); ++arg ) {
		if( Env::GetVar( "COMPILE_FLAGS" ).empty() ) return OK;
		Env::Remove( "COMPILE_FLAGS", * arg, ' ' );
	}
	return OK;
}

EBS_FUNC_FIX_ARG( exec, 1, false, false )
{
	Env::Exec( args[ 0 ], nullptr );
	return OK;
}

EBS_FUNC_FIX_ARG( eval, 1, false, false )
{
	std::string tmp;
	Str::Eval( args[ 0 ], tmp );
	return OK;
}

EBS_FUNC_FIX_ARG( enable_internal_display, 0, false, false )
{
	return IMPLICIT_DISPLAY_ENABLED;
}

EBS_FUNC_FIX_ARG( disable_internal_display, 0, false, false )
{
	return IMPLICIT_DISPLAY_DISABLED;
}

EBS_FUNC_FIX_ARG( brkloop, 0, false, false )
{
	return LOOP_BREAK_ENCOUNTERED;
}

EBS_FUNC_FIX_ARG( contloop, 0, false, false )
{
	return LOOP_CONTINUE_ENCOUNTERED;
}