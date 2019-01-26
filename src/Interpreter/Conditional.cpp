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
#include "../../include/String.hpp"
#include "../../include/IO.hpp"
#include "../../include/Parser.hpp"
#include "../../include/Lexer/Lexer.hpp"
#include "../../include/Parser/BlockStmt.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/Conditional.hpp"

static int EvalConditions( const Parser::Cond & cond, const size_t depth );

int Interpreter::Conditional( const Parser::CondStmt * conds, const size_t depth )
{
	int res = OK;

	IO::colout << "Conditional[" << depth << "]: Interpreting...\n";

	int block_loc = -1;

	for( int i = 0; i < conds->GetConds().size(); ++i ) {
		auto & cond = conds->GetConds()[ i ];
		res = EvalConditions( cond, depth );
		if( res < 0 ) {
			break;
		}
		if( res == 1 ) {
			block_loc = i;
			break;
		}
	}

	if( block_loc < 0 ) {
		IO::colout << "Conditional[" << depth << "]: Could not satisfy any block's conditions! Nothing to do here!\n";
		return OK;
	}

	if( conds->GetConds()[ block_loc ].block != nullptr ) {
		res = Interpreter::Block( conds->GetConds()[ block_loc ].block, depth );
	}

	if( res != FAIL_FN_CALLED ) {
		IO::colout << "Conditional[" << depth << "]: Interpretation finished! ";
		if( res != OK && res != LOOP_BREAK_ENCOUNTERED && res != LOOP_CONTINUE_ENCOUNTERED ) {
			IO::colout << "Errors encountered!\n";
		} else {
			IO::colout << "No errors encountered!\n";
		}
	}
	return res;
}

static int EvalConditions( const Parser::Cond & cond, const size_t depth )
{
	if( cond.ops.empty() ) {
		IO::colout << "Conditional[" << depth << "]: Executing else statement...\n";
		return true;
	}

	bool result = false;
	int res = OK;
	for( int i = 0; i < cond.ops.size(); ) {
		auto & o = cond.ops[ i ];
		//IO::colout << "\tEvaluating: " << o.lhs << " " << o.oper << " " << o.rhs << "\n";
		std::string outl, outr;
		res = Str::Eval( o.lhs, outl );
		if( res != OK ) {
			IO::colout << "Conditional[" << depth << "] {r}error{0}: Failed to fetch LHS from: " << o.lhs << "\n";
			return -1;
		}
		res = Str::Eval( o.rhs, outr );
		if( res != OK ) {
			IO::colout << "Conditional[" << depth << "] {r}error{0}: Failed to fetch RHS from: " << o.rhs << "\n";
			return -1;
		}
		bool result_tmp;
		if( o.oper == Lex::LogicStrs[ Lex::EQ ] ) {
			result_tmp = outl == outr;
		} else if( o.oper == Lex::LogicStrs[ Lex::NE ] ) {
			result_tmp = outl != outr;
		} else if( o.oper == Lex::LogicStrs[ Lex::LT ] ) {
			if( Str::IsNum( outl ) && Str::IsNum( outr ) ) {
				result_tmp = std::stoi( outl ) < std::stoi( outr );
			} else {
				result_tmp = outl < outr;
			}
		} else if( o.oper == Lex::LogicStrs[ Lex::GT ] ) {
			if( Str::IsNum( outl ) && Str::IsNum( outr ) ) {
				result_tmp = std::stoi( outl ) > std::stoi( outr );
			} else {
				result_tmp = outl > outr;
			}
		} else if( o.oper == Lex::LogicStrs[ Lex::LE ] ) {
			if( Str::IsNum( outl ) && Str::IsNum( outr ) ) {
				result_tmp = std::stoi( outl ) <= std::stoi( outr );
			} else {
				result_tmp = outl <= outr;
			}
		} else if( o.oper == Lex::LogicStrs[ Lex::GE ] ) {
			if( Str::IsNum( outl ) && Str::IsNum( outr ) ) {
				result_tmp = std::stoi( outl ) >= std::stoi( outr );
			} else {
				result_tmp = outl >= outr;
			}
		}
		if( i == 0 ) {
			result = result_tmp;
		} else if( o.type == Parser::AND ) {
			result = result && result_tmp;
		} else if( o.type == Parser::OR ) {
			result = result || result_tmp;
		}
		// without this, the code below will attempt to access cond.ops.size()
		// element because of ++tmp_i
		if( i >= cond.ops.size() - 1 ) {
			++i;
			continue;
		}
		// check if the next Operations will allow bypassing of evaluation
		// if so, just throw the result accordingly
		// using this, we can save a lot of time by not evaluating
		// unnecessary operation strings
		int tmp_i = i;
		if( result ) {
			// if result == true, skip all the next OR operations
			while( cond.ops[ ++tmp_i ].type == Parser::OR );
		} else {
			// if result == false, skip all the next AND operations
			while( cond.ops[ ++tmp_i ].type == Parser::AND );
		}
		i = tmp_i;
	}
	if( result ) {
		IO::colout << "Conditional[" << depth << "]: Matched condition(s): ";
		for( auto & o : cond.ops ) {
			if( o.type == Parser::AND ) {
				IO::colout << " && ";
			} else if( o.type == Parser::OR ) {
				IO::colout << " || ";
			}
			IO::colout << "\"" << o.lhs << "\" " << o.oper << " \"" << o.rhs << "\"";
		}
		IO::colout << ", executing it...\n";
	}
	return result ? 1 : 0;
}