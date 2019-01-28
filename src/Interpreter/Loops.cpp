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
	std::string dest;
	if( loop_var->GetArgs().size() > 0 ) dest = loop_var->GetArgs()[ 0 ];
	bool colout_was_enabled = IO::colout.IsEnabled();
	IO::colout.Enable( false );

	std::vector< std::string > final_args;
	// args.size() is bound to be 3 - 4 or 0 because of parse condition
	if( loop_var->GetType() == Parser::LoopType::FOR ) {
		std::string start = "0", end = "0", inc = "0";
		if( loop_var->GetArgs().size() > 0 ) {
			res = Str::Eval( loop_var->GetArgs()[ 1 ], start );
			if( res != OK ) {
				err = "Failed to evaluate the second argument of loop statement";
				goto error;
			}
			res = Str::Eval( loop_var->GetArgs()[ 2 ], end );
			if( res != OK ) {
				err = "Failed to evaluate the third argument of loop statement";
				goto error;
			}
			if( !Str::IsNum( end ) || !Str::IsNum( end ) ) {
				err = "Expected loop range to be numbers!\n";
				goto error;
			}
			if( loop_var->GetArgs().size() == 4 ) {
				res = Str::Eval( loop_var->GetArgs()[ 3 ], inc );
				if( res != OK ) {
					err = "Failed to evaluate the 4th argument of loop statement";
					goto error;
				}
				if( !Str::IsNum( inc ) ) {
					err = "Expected loop increment/decrement to be number!\n";
					goto error;
				}
			} else {
				inc = "1";
			}
		}
		int start_num = std::stoi( start ), end_num = std::stoi( end ), inc_num = std::stoi( inc );
		if( inc_num > 0 ) {
			for( int i = start_num; i <= end_num; i += inc_num ) {
				final_args.push_back( std::to_string( i ) );
			}
		} else if( inc_num < 0 ) {
			for( int i = start_num; i >= end_num; i += inc_num ) {
				final_args.push_back( std::to_string( i ) );
			}
		}
		if( loop_var->GetArgs().size() > 0 ) goto finite_loop;
		else goto infinite_loop;
	} else if( loop_var->GetType() == Parser::LoopType::FOREACH ) { // args.size() will be >= 2
		for( auto it = loop_var->GetArgs().begin() + 1; it != loop_var->GetArgs().end(); ++it ) {
			std::string tmp;
			res = Str::Eval( * it, tmp );
			if( res != OK ) {
				err = "Failed to evaulate foreach argument: " + ( * it );
				goto error;
			}
			final_args.push_back( tmp );
		}
		goto finite_loop;
	} else if( loop_var->GetType() == Parser::LoopType::FOREACHVAR ) { // args.size() will be = 2 or 3
		std::string val = Env::GetVar( loop_var->GetArgs()[ 0 ] );
		char delim = ' ';
		if( loop_var->GetArgs().size() > 2 && !loop_var->GetArgs()[ 2 ].empty() ) {
			delim = loop_var->GetArgs()[ 2 ][ 0 ];
		}
		final_args = Str::Delimit( val, delim );
		goto finite_loop;
	}

infinite_loop:
	while( true ) {
		if( loop_var->GetBlock() != nullptr ) res = Interpreter::Block( loop_var->GetBlock(), depth );
		if( res != OK && res != LOOP_CONTINUE_ENCOUNTERED ) break;
	}
	goto error;

finite_loop:
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
error:
	IO::colout.Enable( colout_was_enabled );
	Env::Reset( dest );
	if( res == LOOP_BREAK_ENCOUNTERED || res == LOOP_CONTINUE_ENCOUNTERED ) res = OK;
	if( res != OK && res != FAIL_FN_CALLED ) {
		IO::colout << "Loop call[" << depth << "] {r}error{0}(" << res << "): Failed to interpret, " + err + "!{0}\n";
	}
	return res;
}