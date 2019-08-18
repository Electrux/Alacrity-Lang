/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <vector>
#include <string>

#include "../include/Errors.hpp"
#include "../include/Env.hpp"
#include "../include/FS.hpp"
#include "../include/DynLib.hpp"
#include "../include/Core.hpp"

#define _STRINGIZE(x) #x
#define STRINGIFY(x) _STRINGIZE(x)

std::string Core::ALLibPaths()
{
	return "AL_LIB_PATHS";
}

std::string Core::ALSourcePaths()
{
	return "AL_SRC_PATHS";
}

std::string Core::FuncLibFile( const std::string & func_name )
{
	return "lib" + func_name + ".so";
}

int Core::Init( const int argc, const char ** argv )
{
	if( !Env::SetVar( ALLibPaths(), STRINGIFY( BUILD_PREFIX_DIR ) "/share/allang_libs/" ) ) {
		std::cout << "Core::Init() failed: Unable to set core AL_LIB_PATHS env var!\n";
		return ENV_SETVAR_FAILED;
	}

	if( !Env::SetVar( ALSourcePaths(), STRINGIFY( BUILD_PREFIX_DIR ) "/share/allang_tests/" ) ) {
		std::cout << "Core::Init() failed: Unable to set core AL_SRC_PATHS env var!\n";
		return ENV_SETVAR_FAILED;
	}

#if __ANDROID__
	if( !Env::SetVar( "OS", "OS_ANDROID" ) ) {
#elif __linux__
	if( !Env::SetVar( "OS", "OS_LINUX" ) ) {
#elif __APPLE__
	if( !Env::SetVar( "OS", "OS_OSX" ) ) {
#elif __FreeBSD__ || __NetBSD__ || __OpenBSD__ || __bsdi__ || __DragonFly__
	if( !Env::SetVar( "OS", "OS_BSD" ) ) {
#endif
		std::cout << "Core::Init() failed: Unable to set initial OS env var!\n";
		return ENV_SETVAR_FAILED;
	}

	if( !Env::SetVar( "UID", std::to_string( Env::GetUID() ) ) ) {
		std::cout << "Core::Init() failed: Unable to set initial UID env var!\n";
		return ENV_SETVAR_FAILED;
	}

	if( !Env::SetVar( "IS_ROOT", Env::GetUID() == 0 ? "true" : "false" ) ) {
		std::cout << "Core::Init() failed: Unable to set initial IS_ROOT env var!\n";
		return ENV_SETVAR_FAILED;
	}

	// Set argument variables
	for( int i = 2; i < argc; ++i ) {
		if( !Env::SetVar( "ARG_" + std::to_string( i - 2 ), argv[ i ] ) ) {
			std::cout << "Core::Init() failed: Unable to set initial ARG_" << i - 2 << " env var!\n";
			return ENV_SETVAR_FAILED;
		}
	}
	if( !Env::SetVar( "ARGC", std::to_string( argc - 2 ) ) ) {
		std::cout << "Core::Init() failed: Unable to set initial ARGC env var!\n";
		return ENV_SETVAR_FAILED;
	}

	return OK;
}

void Core::DeInit()
{
	// Nothing here for now
}