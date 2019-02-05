/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <iostream>

#include "../include/Errors.hpp"
#include "../include/Env.hpp"
#include "../include/FS.hpp"
#include "../include/IO.hpp"
#include "../include/DynLib.hpp"
#include "../include/Parser.hpp"
#include "../include/Interpreter/Block.hpp"
#include "../include/Interpreter/Loops.hpp"
#include "../include/Interpreter/FuncCall.hpp"
#include "../include/Interpreter/Assignment.hpp"
#include "../include/Interpreter/Conditional.hpp"
#include "../include/Interpreter.hpp"

int Interpreter::Interpret( const Parser::ParseTree & ps, const std::string & file_path,
			const int depth, const bool enable_internal_display )
{
	IO::colout.Enable( enable_internal_display );
	IO::colout << "Entering [{c}" << file_path << "{0}]...\n";
	int res = OK;

	auto cwd = FS::GetCurrentDir();
	auto srcpath = Env::GetDirPart( file_path );
	if( srcpath != "." ) {
		FS::SetCurrentDir( srcpath );
	}

	auto prev_file = Env::GetVar( "CURRENT_FILE" );
	Env::SetVar( "CURRENT_FILE", file_path );

	for( auto it = ps.GetStmts().begin(); it != ps.GetStmts().end(); ++it ) {
		auto & stmt = * it;
		if( stmt->GetType() == Parser::ASSIGN ) {
			res = Interpreter::Assignment( static_cast< const Parser::AssignStmt * >( stmt ), depth + 1 );
		} else if( stmt->GetType() == Parser::FNCALL ) {
			res = Interpreter::FuncCall( static_cast< const Parser::FnCallStmt * >( stmt ), depth + 1 );
		} else if( stmt->GetType() == Parser::BLOCK ) {
			res = Interpreter::Block( static_cast< const Parser::BlockStmt * >( stmt ), depth + 1 );
		} else if( stmt->GetType() == Parser::COND ) {
			res = Interpreter::Conditional( static_cast< const Parser::CondStmt * >( stmt ), depth + 1 );
		} else if( stmt->GetType() == Parser::LOOP ) {
			res = Interpreter::LoopCall( static_cast< const Parser::LoopStmt * >( stmt ), depth + 1 );
		} else {
			IO::colout << "Interpret[" << depth << "] {r}error{0}: Unrecognized object type with value: " << stmt->GetType() << "\n";
			res = UNKNOWN_OBJ_TYPE;
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
	FS::SetCurrentDir( cwd );

	Env::SetVar( "CURRENT_FILE", prev_file );

	if( res != FAIL_FN_CALLED ) {
		IO::colout << "Exited [{c}" << file_path << "{0}]! ";
		if( res != OK ) {
			IO::colout << "Errors encountered!\n";
		} else {
			IO::colout << "No errors encountered!\n";
		}
	}
	DynLib::Deinit();
	return res;
}