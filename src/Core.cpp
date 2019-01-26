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

std::string Core::BSFuncPaths()
{
	return "BS_FUNC_PATHS";
}

std::string Core::BSSearchPaths()
{
	return "BS_SEARCH_PATHS";
}

std::string Core::FuncLibFile( const std::string & func_name )
{
	return "lib" + func_name + ".so";
}

std::string Core::DefaultLicense()
{
	return "bsd3";
}

int Core::Init()
{
	int res = OK;

	if( !Env::SetVar( BSFuncPaths(), FS::GetCurrentDir() + "/lib" ) ) {
		std::cout << "Core::Init() failed: Unable to set core BS_FUNC_PATHS env var!\n";
		return ENV_SETVAR_FAILED;
	}

#ifdef __linux__
	if( !Env::SetVar( "OS", "OS_LINUX" ) ) {
#elif __APPLE__
	if( !Env::SetVar( "OS", "OS_OSX" ) ) {
#endif
		std::cout << "Core::Init() failed: Unable to set initial OS env var!\n";
		return ENV_SETVAR_FAILED;
	}

	if( !Env::SetVar( "PROJECT_NAME", "default" ) ) {
		std::cout << "Core::Init() failed: Unable to set initial PROJECT_NAME env var!\n";
		return ENV_SETVAR_FAILED;
	}

	if( !Env::SetVar( "PROJECT_VERSION", "0.0.1" ) ) {
		std::cout << "Core::Init() failed: Unable to set initial PROJECT_VERSION env var!\n";
		return ENV_SETVAR_FAILED;
	}

	if( !Env::SetVar( "LANGUAGE", "cpp" ) ) {
		std::cout << "Core::Init() failed: Unable to set initial LANGUAGE env var!\n";
		return ENV_SETVAR_FAILED;
	}

#if __linux__
	if( Env::GetVar( "CC" ).empty() && !Env::SetVar( "CC", Env::GetVar( "LANGUAGE" ) == "cpp" ? "g++" : "gcc" ) ) {
#elif __APPLE__
	if( Env::GetVar( "CC" ).empty() && !Env::SetVar( "CC", Env::GetVar( "LANGUAGE" ) == "cpp" ? "clang++" : "clang" ) ) {
#endif
		std::cout << "Core::Init() failed: Unable to set initial CC env var!\n";
		return ENV_SETVAR_FAILED;
	}

	if( !Env::SetVar( "STD_VERSION", "11" ) ) {
		std::cout << "Core::Init() failed: Unable to set initial STD_VERSION env var!\n";
		return ENV_SETVAR_FAILED;
	}

	if( !Env::SetVar( "LICENSE", DefaultLicense() ) ) {
		std::cout << "Core::Init() failed: Unable to set initial LICENSE env var!\n";
		return ENV_SETVAR_FAILED;
	}

	if( !Env::SetVar( "AUTHOR_NAME", "unknown" ) ) {
		std::cout << "Core::Init() failed: Unable to set initial AUTHOR_NAME env var!\n";
		return ENV_SETVAR_FAILED;
	}

	if( !Env::SetVar( "AUTHOR_EMAIL", "unknown@unknown" ) ) {
		std::cout << "Core::Init() failed: Unable to set initial AUTHOR_EMAIL env var!\n";
		return ENV_SETVAR_FAILED;
	}

	return res;
}

void Core::DeInit()
{
	// Nothing here for now
}