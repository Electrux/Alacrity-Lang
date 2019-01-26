/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef INTERPRETER_FN_BASE_HPP
#define INTERPRETER_FN_BASE_HPP

#include <vector>
#include <string>

#include "../Parser.hpp"
#include "../Parser/BlockStmt.hpp"

struct FnInfo
{
	const char * name;
	const int min_args;
	const int max_args;
	const bool persist_env;
	const bool use_block;
};

#define EBS_FUNC_VAR_ARG( fn_name, fn_min_args, fn_max_args,				\
			fn_persist_env, fn_use_block )					\
static_assert( fn_min_args >= 0 && ( fn_max_args > fn_min_args || fn_max_args == -1 ),	\
	"Min args must be >= zero and not equal to Max args, or Max must be == -1 " );	\
extern "C" {										\
	FnInfo f_inf_##fn_name = { .name = #fn_name,					\
				.min_args = fn_min_args,				\
				.max_args = fn_max_args,				\
				.persist_env = fn_persist_env,				\
				.use_block = fn_use_block };				\
}											\
extern "C" int fn_##fn_name( const std::vector< std::string > & args, const int depth,	\
				const Parser::BlockStmt * block,			\
				const bool internal_display_enabled )

#define EBS_FUNC_FIX_ARG( fn_name, fn_arg_count, fn_persist_env, fn_use_block )		\
static_assert( fn_arg_count >= -1, "Argument count must be >= -1" );			\
extern "C" {										\
	FnInfo f_inf_##fn_name = { .name = #fn_name,					\
				.min_args = fn_arg_count,				\
				.max_args = fn_arg_count,				\
				.persist_env = fn_persist_env,				\
				.use_block = fn_use_block };				\
}											\
extern "C" int fn_##fn_name( const std::vector< std::string > & args, const int depth,	\
				const Parser::BlockStmt * block,			\
				const bool internal_display_enabled )

#endif // INTERPRETER_FN_BASE_HPP