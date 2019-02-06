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
#include <fstream>
#include <regex>
#include <sys/stat.h>

#include "../../include/Errors.hpp"
#include "../../include/String.hpp"
#include "../../include/Env.hpp"
#include "../../include/IO.hpp"
#include "../../include/FS.hpp"
#include "../../include/Interpreter.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

#include "builds.hpp"

bool IsFileLatest( const std::string & file1, const std::string & file2 );

AL_FUNC_VAR_ARG( builds, 1, 2, false, false )
{
	auto builder = Builder::Get();
	builder->Reset();

	int res = builder->SetBuildType( args[ 0 ] );
	if( res != OK ) return res;
	if( args.size() >= 2 ) res = builder->SetLibType( args[ 1 ] );
	if( res != OK ) return res;

	return OK;
}

AL_FUNC_VAR_ARG( sources, 1, -1, false, false )
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

AL_FUNC_VAR_ARG( build, 2, -1, false, false )
{
	Env::Backup();
	auto builder = Builder::Get();
	auto builder_backup = * builder;

	auto main_src_vec = Str::Delimit( args[ 1 ], ',' );
	Str::RemoveAllPathsWithRegex( builder->Sources(), main_src_vec[ 0 ] );
	builder->MainSource() = main_src_vec[ 0 ];

	for( auto it = main_src_vec.begin() + 1; it < main_src_vec.end(); ++it ) {
		auto tmp_srcs = FS::GetFilesFromRegex( * it );
		for( auto & tmp_src : tmp_srcs ) {
			if( std::find( builder->Sources().begin(), builder->Sources().end(), tmp_src ) == builder->Sources().end() )
				builder->Sources().push_back( tmp_src );
		}
	}

	Env::SetVar( "TARGET", args[ 0 ] );
	Env::SetVar( "BUILD_TYPE", builder->GetBuildType() );

	if( args.size() >= 3 ) {
		if( Env::GetVar( "LANGUAGE" ) == "c++" ) 	Env::Append( "CXX_INC_DIRS", args[ 2 ], ' ' );
		else 						Env::Append( "C_INC_DIRS", args[ 2 ], ' ' );
	}
	if( args.size() >= 4 ) {
		if( Env::GetVar( "LANGUAGE" ) == "c++" ) 	Env::Append( "CXX_LIB_FLAGS", args[ 3 ], ' ' );
		else 						Env::Append( "C_LIB_FLAGS", args[ 3 ], ' ' );
	}
	if( args.size() >= 5 ) {
		if( Env::GetVar( "LANGUAGE" ) == "c++" ) 	Env::Append( "CXX_FLAGS", args[ 4 ], ' ' );
		else 						Env::Append( "C_FLAGS", args[ 4 ], ' ' );
	}

	IO::colout << "{bm}Building target{0}: {by}${TARGET}{0} {bm}of type{0}: {by}${BUILD_TYPE}{0} ...\n";

	std::string base_msg_str = "{m}Compiling ${LANGUAGE} object{0}: {y}buildfiles/${SRC}.o{0} ...";
	std::string base_comp_str = "${CC} -std=${LANGUAGE}${STD_VERSION} ";

	if( Env::GetVar( "LANGUAGE" ) == "c++" ) base_comp_str += "${CXX_FLAGS} ${CXX_INC_DIRS} -c ";
	else base_comp_str += "${C_FLAGS} ${C_INC_DIRS} -c ";

	base_comp_str += "${SRC} -o buildfiles/${SRC}.o ";

	std::vector< Env::ExecData > cmds;

	Env::Reset( "ALL_SRCS" );

	bool cmds_only = Env::GetVar( "CMDS_ONLY" ) == "true";

	for( auto & src : builder->Sources() ) {
		Env::SetVar( "SRC", src );
		std::string comp_str, msg_str;
		EVAL_AND_CHECK( "build", base_comp_str, comp_str );
		EVAL_AND_CHECK( "build", base_msg_str, msg_str );

		Env::Append( "ALL_SRCS", "buildfiles/" + src + ".o", ' ' );

		if( cmds_only ) {
			IO::colout << comp_str << "\n";
			continue;
		}

		if( IsFileLatest( "buildfiles/" + src + ".o", src ) ) continue;
		if( !FS::CreateDirectoriesForFile( "buildfiles/" + src ) ) return FAIL;
		cmds.push_back( { msg_str, comp_str } );
	}

	if( !builder->MainSource().empty() ) {
		Env::SetVar( "SRC", builder->MainSource() );
		std::string comp_str, msg_str;
		std::string base_target_obj_str, target_obj_str;

		if( builder->GetBuildType() == "bin" || builder->GetBuildType() == "test" || builder->GetLibType() == "dynamic" ) {
			base_comp_str = "${CC} ";

			if( builder->GetLibType() == "dynamic" ) base_comp_str += "-shared ";

			base_comp_str += "-std=${LANGUAGE}${STD_VERSION} ";

			if( Env::GetVar( "LANGUAGE" ) == "c++" ) base_comp_str += "${CXX_FLAGS} ${CXX_INC_DIRS} ";
			else base_comp_str += "${C_FLAGS} ${C_INC_DIRS} ";

			if( builder->GetBuildType() == "bin" || builder->GetBuildType() == "test" ) base_comp_str += "-g ";

			base_comp_str += "${SRC} ${ALL_SRCS} -o buildfiles/";
			if( builder->GetBuildType() == "lib" ) {
				base_target_obj_str = "buildfiles/lib${TARGET}.so";
				base_comp_str += "lib${TARGET}.so ";
			} else {
				base_target_obj_str = "buildfiles/${TARGET}";
				base_comp_str += "${TARGET} ";
			}

			if( Env::GetVar( "LANGUAGE" ) == "c++" ) base_comp_str += "${CXX_LIB_DIRS} ${CXX_LIB_FLAGS}";
			else base_comp_str += "${C_LIB_DIRS} ${C_LIB_FLAGS}";

			EVAL_AND_CHECK( "build", base_comp_str, comp_str );
			base_msg_str = "{bm}Building target{0}: {by}buildfiles/";
			if( builder->GetLibType() == "dynamic" ) base_msg_str += "lib${TARGET}.so";
			else base_msg_str += "${TARGET}";
			base_msg_str += "{0} ...";
		} else if( builder->GetLibType() == "static" ) {
			base_comp_str = "ar rcs buildfiles/lib${TARGET}.a ${SRC} ${ALL_SRCS} ${CXX_LIB_DIRS} ${CXX_LIB_FLAGS}";
			base_target_obj_str = "lib${TARGET}.a";
			std::string comp_str;
			EVAL_AND_CHECK( "build", base_comp_str, comp_str );
			base_msg_str = "{bm}Building target{0}: {by}buildfiles/lib${TARGET}.a{0} ...";
		}
		EVAL_AND_CHECK( "build", base_msg_str, msg_str );
		EVAL_AND_CHECK( "build", base_target_obj_str, target_obj_str );

		if( cmds_only ) IO::colout << comp_str << "\n";
		if( !cmds_only && ( cmds.size() > 0 || !IsFileLatest( target_obj_str, builder->MainSource() )
				|| !IsFileLatest( target_obj_str, Env::GetVar( "CURRENT_FILE" ) ) ) ) {
			if( !FS::CreateDirectoriesForFile( "buildfiles/" + builder->MainSource() ) ) return FAIL;
			cmds.push_back( { msg_str, comp_str } );
		}
	}

	// Finally, build all
	int res = !cmds_only ? Env::MultiThreadedExec( cmds ) : OK;

	Env::Reset( "TARGET" );
	* builder = builder_backup;
	Env::Restore();
	return res;
}

bool IsFileLatest( const std::string & file1, const std::string & file2 )
{
	if( !FS::LocExists( file1 ) ) return false;

	struct stat info1, info2;

	if( stat( file1.c_str(), & info1 ) != 0 ) {
		return false;
	}

	if( stat( file2.c_str(), & info2 ) != 0 ) {
		return false;
	}

	if( info1.st_mtime < info2.st_mtime ) {
		return false;
	}

	std::fstream f2;

	f2.open( file2, std::ios::in );
	if( !f2 ) {
		return false;
	}

	std::string line;

	std::regex inc_regex( "#include \"(.*)\"" );
	std::smatch match;

	auto last_slash = file2.rfind( '/' );
	std::string dir;
	if( last_slash != std::string::npos ) dir = file2.substr( 0, last_slash );

	while( std::getline( f2, line ) ) {
		if( std::regex_search( line, match, inc_regex ) ) {
			struct stat temp_info;
			std::string m = dir + "/" + std::string( match[ 1 ] );

			if( stat( m.c_str(), & temp_info ) != 0 ) continue;

			if( !FS::LocExists( m ) ) {
				f2.close();
				return false;
			}

			if( temp_info.st_mtime >= info1.st_mtime ) {
				f2.close();
				return false;
			}
		}
	}

	f2.close();
	return true;
}