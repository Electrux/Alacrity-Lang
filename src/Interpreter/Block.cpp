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
#include "../../include/IO.hpp"
#include "../../include/Parser.hpp"
#include "../../include/Parser/BlockStmt.hpp"
#include "../../include/Interpreter/FuncCall.hpp"
#include "../../include/Interpreter/Conditional.hpp"
#include "../../include/Interpreter/Loops.hpp"
#include "../../include/Interpreter/Block.hpp"

int Interpreter::Block( const Parser::BlockStmt * blk, const size_t depth )
{
	int res = OK;

	if( blk->size() == 0 ) {
		IO::colout << "Block[" << depth << "]: Nothing to do!\n";
		return res;
	}

	IO::colout << "Block[" << depth << "]: Entering...\n";

	for( auto & stmt : blk->GetStmts() ) {
		if( stmt->GetType() == Parser::FNCALL ) {
			res = Interpreter::FuncCall( static_cast< const Parser::FnCallStmt * >( stmt ), depth + 1 );
		} else if( stmt->GetType() == Parser::BLOCK ) {
			res = Interpreter::Block( static_cast< const Parser::BlockStmt * >( stmt ), depth + 1 );
		} else if( stmt->GetType() == Parser::COND ) {
			res = Interpreter::Conditional( static_cast< const Parser::CondStmt * >( stmt ), depth + 1 );
		} else if( stmt->GetType() == Parser::LOOP ) {
			res = Interpreter::LoopCall( static_cast< const Parser::LoopStmt * >( stmt ), depth + 1 );
		} else if( stmt->GetType() == Parser::LOOP_IN_VAR ) {
			res = Interpreter::LoopInVarCall( static_cast< const Parser::LoopInVarStmt * >( stmt ), depth + 1 );
		} else {
			IO::colout << "Interpret[" << depth << "] {r}error{0}: Unrecognized object type with value: "
				<< stmt->GetType() << "\n";
			res = UNKNOWN_OBJ_TYPE;
			break;
		}
		if( res == LOOP_BREAK_ENCOUNTERED || res == LOOP_CONTINUE_ENCOUNTERED ) {
			break;
		}
		if( res != OK ) {
			if( res != FAIL_FN_CALLED ) {
				IO::colout << "Interpret[" << depth << "] {r}error{0}: Failed to interpret object "
					<< stmt->GetType() << ", look at error above!\n";
			}
			break;
		}
	}
	if( res != FAIL_FN_CALLED ) {
		IO::colout << "Block[" << depth << "]: Exiting! ";
		if( res != OK && res != LOOP_BREAK_ENCOUNTERED && res != LOOP_CONTINUE_ENCOUNTERED ) {
			IO::colout << "Errors encountered!\n";
		} else {
			IO::colout << "No errors encountered!\n";
		}
	}
	return res;
}