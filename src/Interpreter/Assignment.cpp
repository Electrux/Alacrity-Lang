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
#include "../../include/Parser/AssignStmt.hpp"
#include "../../include/Interpreter/FnBase.hpp"
#include "../../include/Interpreter/Assignment.hpp"

int Interpreter::Assignment( const Parser::AssignStmt * assn_var, const size_t depth )
{
	int res = OK;
	std::string err;

	auto var = assn_var->GetVar();
	auto val = assn_var->GetVal();

	std::string var_final, val_final;
	res = Str::Eval( var, var_final );
	if( res != OK ) {
		err = "Failed evaluating variable: " + var;
		goto error;
	}

	res = Str::Eval( val, val_final );
	if( res != OK ) {
		err = "Failed evaluating value: " + val + " for variable: " + var;
		goto error;
	}

	Env::SetVar( var_final, val_final );
error:
	if( res != OK ) {
		IO::colout << "Assignment[" << depth << "] {r}error{0}(" << res << "): Failed to interpret, " + err + "!{0}\n";
	}
	return res;
}