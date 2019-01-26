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
#include "../../include/String.hpp"
#include "../../include/Parser.hpp"
#include "../../include/Parser/BlockStmt.hpp"
#include "../../include/Interpreter/FnBase.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/Loops.hpp"

int Interpreter::LoopCall( const Parser::LoopStmt * loop_var, const size_t depth )
{
	int res = OK;
	std::string err;

	// Temporary variable for loop
	std::string dest = loop_var->GetArgs()[ 0 ];
	bool colout_was_enabled = IO::colout.IsEnabled();

	std::vector< std::string > final_args;
	if( loop_var->GetArgs().size() == 3 ) {
		std::string arg1, arg2;
		res = Str::Eval( loop_var->GetArgs()[ 1 ], arg1 );
		if( res != OK ) {
			err = "Failed to evaluate the second argument of loop statement";
			goto error;
		}
		res = Str::Eval( loop_var->GetArgs()[ 2 ], arg2 );
		if( res != OK ) {
			err = "Failed to evaluate the third argument of loop statement";
			goto error;
		}
		if( Str::IsNum( arg1 ) && Str::IsNum( arg2 ) ) {
			int r1 = std::stoi( arg1 );
			int r2 = std::stoi( arg2 );
			if( r1 > r2 ) {
				for( int i = r1; i >= r2; --i ) {
					final_args.push_back( std::to_string( i ) );
				}
			} else {
				for( int i = r1; i <= r2; ++i ) {
					final_args.push_back( std::to_string( i ) );
				}
			}
		} else {
			for( auto it = loop_var->GetArgs().begin() + 1; it != loop_var->GetArgs().end(); ++it ) {
				final_args.push_back( * it );
			}
		}
	} else {
		for( auto it = loop_var->GetArgs().begin() + 1; it != loop_var->GetArgs().end(); ++it ) {
			final_args.push_back( * it );
		}
	}

	IO::colout.Enable( false );
	for( auto & var : final_args ) {
		std::string arg;
		res = Str::Eval( var, arg );
		if( res != OK ) {
			err = "Failed to evaluate an argument of loop statement";
			goto error;
		}
		Env::SetVar( dest, arg );
		if( loop_var->GetBlock() != nullptr ) res = Interpreter::Block( loop_var->GetBlock(), depth );
		if( res != OK && res != LOOP_CONTINUE_ENCOUNTERED ) break;
	}
	IO::colout.Enable( colout_was_enabled );
	Env::Reset( dest );
	if( res == LOOP_BREAK_ENCOUNTERED || res == LOOP_CONTINUE_ENCOUNTERED ) res = OK;
error:
	if( res != OK && res != FAIL_FN_CALLED ) {
		IO::colout << "Foreach call[" << depth << "] {r}error{0}(" << res << "): Failed to interpret, " + err + "!{0}\n";
	}
	return res;
}

int Interpreter::LoopInVarCall( const Parser::LoopInVarStmt * loop_var, const size_t depth )
{
	int res = OK;
	std::string err;
	bool reverse_loop = false;

	// Temporary variable for loop
	std::string dest = loop_var->GetArgs()[ 0 ];

	// use variable, from variable, delim
	auto val = Env::GetVar( loop_var->GetArgs()[ 1 ] );
	if( val.empty() ) return OK;
	auto vals = Str::Delimit( val, loop_var->GetArgs().size() == 2 ? ' ' : loop_var->GetArgs()[ 2 ][ 0 ] );

	if( loop_var->GetArgs().size() == 4 && ( loop_var->GetArgs()[ 3 ] == "1" || loop_var->GetArgs()[ 3 ] == "true" ) ) reverse_loop = true;

	IO::colout.Enable( false );
	if( reverse_loop ) {
		for( auto it = vals.rbegin(); it != vals.rend(); ++it ) {
			Env::SetVar( dest, * it );
			if( loop_var->GetBlock() != nullptr ) res = Interpreter::Block( loop_var->GetBlock(), depth );
			if( res != OK ) break;
		}
	} else {
		for( auto it = vals.begin(); it != vals.end(); ++it ) {
			Env::SetVar( dest, * it );
			if( loop_var->GetBlock() != nullptr ) res = Interpreter::Block( loop_var->GetBlock(), depth );
			if( res != OK ) break;
		}
	}
	IO::colout.Enable( true );
	Env::Reset( dest );
error:
	if( res != OK && res != FAIL_FN_CALLED ) {
		IO::colout << "Foreach call[" << depth << "] {r}error{0}(" << res << "): Failed to interpret, " + err + "!{0}\n";
	}
	return res;
}