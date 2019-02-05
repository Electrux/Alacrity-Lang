/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef FS_HPP
#define FS_HPP

#include <vector>
#include <string>
#include <variant>

namespace FS
{
bool LocExists( const std::string & loc );

std::string GetCurrentDir();
void SetCurrentDir( const std::string & path );

std::variant< int, std::string > ReadFile( const std::string & file_name );

bool CreateDirectoriesForFile( const std::string & file );

std::string GetFilePath( const std::string & file, const std::string & env_var = "", const char delim = ':' );

std::vector< std::string > GetFilesFromRegex( std::string regex_str );

bool DeleteFile( const std::string & file );
}

#endif // FS_HPP
