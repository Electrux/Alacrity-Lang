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

std::string Core::FuncLibFile( const std::string & func_name )
{
	return "lib" + func_name + ".so";
}

int Core::Init()
{
	int res = OK;

	if( !Env::SetVar( BSFuncPaths(), "lib" ) ) {
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
	return OK;
}

void Core::DeInit()
{
	// Nothing here for now
}