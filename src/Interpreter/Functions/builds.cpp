/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <iostream>
#include <vector>
#include <string>

#include "../../../include/Errors.hpp"
#include "../../../include/String.hpp"
#include "../../../include/Env.hpp"
#include "../../../include/FS.hpp"
#include "../../../include/IO.hpp"
#include "../../../include/Parser.hpp"
#include "../../../include/Interpreter/FnBase.hpp"
#include "builds.hpp"

struct CompileData
{
	std::string cmd;
	std::string dest;
};

int CompileAll( const std::vector< CompileData > & cdata );

EBS_FUNC_VAR_ARG( builds, 1, 2, false, false )
{
	int res = OK;
	auto builder = Builder::Get();
	builder->Reset();

	res = builder->SetBuildType( args[ 0 ] );
	if( res != OK ) return res;
	if( args.size() >= 2 ) res = builder->SetLibType( args[ 1 ] );
	if( res != OK ) return res;

	return res;
}

EBS_FUNC_VAR_ARG( sources, 1, -1, false, false )
{
	std::vector< std::string > srcs;
	auto builder = Builder::Get();
	for( auto & arg : args ) {
		if( arg.empty() ) continue;
		if( arg[ 0 ] == '-' ) {
			Str::RemoveAllPathsWithRegex( builder->Sources(), arg.substr( 1 ) );
			continue;
		}
		srcs = FS::GetFilesFromRegex( arg );
		if( srcs.empty() ) continue;
		builder->Sources().insert( builder->Sources().end(), srcs.begin(), srcs.end() );
	}
	return OK;
}

EBS_FUNC_VAR_ARG( build, 2, -1, false, false )
{
	int res = OK;
	Env::Backup();
	auto builder = Builder::Get();
	auto builder_backup = * builder;
	for( auto arg = args.begin() + 2; arg < args.end(); ++arg ) {
		if( arg->empty() ) continue;
		if( ( * arg )[ 0 ] == '-' ) {
			Str::RemoveAllPathsWithRegex( builder->Sources(), arg->substr( 1 ) );
			continue;
		}
		auto srcs = FS::GetFilesFromRegex( * arg );
		if( srcs.empty() ) continue;
		builder->Sources().insert( builder->Sources().end(), srcs.begin(), srcs.end() );
	}
	builder->MainSources().push_back( args[ 1 ] );
	Env::SetVar( "TARGET", args[ 0 ] );
	if( Env::GetVar( "LANGUAGE" ) == "cpp" ) {
		Env::SetVar( "LANGUAGE_CODE", "c++" );
	} else {
		Env::SetVar( "LANGUAGE_CODE", Env::GetVar( "LANGUAGE" ) );
	}

	IO::colout( true ) << "{bm}Building target{0}: {by}${TARGET}{0}...\n";
	auto internal_build_path = Env::GetVar( "INTERNAL_BUILD_PATH" );
	std::vector< CompileData > compile_cmds;
	for( auto & main_src : builder->MainSources() ) {
		compile_cmds.clear();
		std::string main_dest = main_src;
		auto main_loc = main_dest.find( internal_build_path );
		if( main_loc != std::string::npos ) {
			main_dest.erase( main_loc, main_loc + internal_build_path.size() );
		}
		if( !FS::CreateDirectoriesForFile( internal_build_path + "/buildfiles/" + main_dest ) ) {
			res = DIR_CREATE_FAILED;
			goto done;
		}
		for( auto src : builder->Sources() ) {
			std::string dest = src;
			auto loc = dest.find( internal_build_path );
			if( loc != std::string::npos ) {
				dest.erase( loc, loc + internal_build_path.size() );
				if( * dest.begin() == '/' || * dest.begin() == '.' ) {
					dest.erase( dest.begin() );
				}
			}
			if( !FS::CreateDirectoriesForFile( internal_build_path + "/buildfiles/" + dest ) ) {
				res = DIR_CREATE_FAILED;
				goto done;
			}
			std::string op;
			Str::Eval( "${CC} -std=${LANGUAGE_CODE}${STD_VERSION} -c ${CXX_FLAGS} ${CXX_INC_DIRS} " + src
				+ " -o ${INTERNAL_BUILD_PATH}/buildfiles/" + dest + ".o", op );
			Env::Append( "OTHER_SOURCES", "buildfiles/" + dest + ".o", ' ' );
			compile_cmds.push_back( { op, dest + ".o" } );
		}
		// Last element in compile_cmds is main src
		std::string op;
		std::string eval_str;
		Env::SetVar( "BUILD_TYPE", builder->GetBuildType() );
		Env::SetVar( "LIB_TYPE", builder->GetLibType() );
		std::string ext;
		if( builder->GetBuildType() == "binary" || builder->GetBuildType() == "test" ) {
			eval_str = "${CC} -std=${LANGUAGE_CODE}${STD_VERSION} -g ${CXX_FLAGS} ${CXX_INC_DIRS} " + main_src
				+ " -o ${INTERNAL_BUILD_PATH}/buildfiles/" + main_dest + " ${OTHER_SOURCES} ${CXX_LIB_DIRS} ${CXX_LIB_FLAGS}";
		} else if( builder->GetBuildType() == "library" ) {
			if( builder->GetLibType() == "dynamic" ) {
				eval_str = "${CC} -shared -std=${LANGUAGE_CODE}${STD_VERSION} -g ${CXX_FLAGS} ${CXX_INC_DIRS} " + main_src
					+ " -o ${INTERNAL_BUILD_PATH}/buildfiles/" + main_dest + ".so ${OTHER_SOURCES} ${CXX_LIB_DIRS} ${CXX_LIB_FLAGS}";
				ext = ".so";
			} else if( builder->GetLibType() == "static" ) {
				eval_str = "ar rcs ${INTERNAL_BUILD_PATH}/buildfiles/" + main_dest + ".a ${OTHER_SOURCES} ${CXX_LIB_DIRS} ${CXX_LIB_FLAGS}";
				ext = ".a";
			}
		}
		Str::Eval( eval_str, op );
		compile_cmds.push_back( { op, main_dest + ext } );
		CompileAll( compile_cmds );
	}

done:
	Env::Reset( "TARGET" );
	* builder = builder_backup;
	Env::Restore();
	return res;
}

int CompileAll( const std::vector< CompileData > & cdata )
{
	auto lang = Env::GetVar( "LANGUAGE" );
	if( lang == "cpp" ) {
		Env::SetVar( "OBJ_TYPE", "CXX" );
	} else if( lang == "c" ) {
		Env::SetVar( "OBJ_TYPE", "C" );
	}
	for( auto c = cdata.begin(); c != cdata.end(); ++c ) {
		if( c == cdata.end() - 1 ) IO::colout << "{bm}Building{0}: {bb}" << c->dest << "{0} ...\n";
		else IO::colout( true ) << "{m}Compiling ${OBJ_TYPE} object{0}: {b}" << c->dest << "{0} ...\n";
		Env::Exec( c->cmd );
	}
	return OK;
}