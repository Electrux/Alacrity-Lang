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
#include <filesystem>
#include <regex>
#include <variant>
#include <cstdio>
#include <sys/stat.h>

#include "../include/Env.hpp"
#include "../include/FS.hpp"

bool FS::LocExists( const std::string & loc )
{
	if( loc.empty() ) return true;

	struct stat info;
	if( stat( loc.c_str(), & info ) == 0 || lstat( loc.c_str(), & info ) == 0 ) {
		return true;
	}
	return false;
}

std::string FS::GetCurrentDir()
{
	return std::filesystem::current_path().string();
}

void FS::SetCurrentDir( const std::string & path )
{
	std::filesystem::current_path( path );
}

std::variant< int, std::string > FS::ReadFile( const std::string & file_name )
{
	if( !LocExists( file_name ) ) {
		std::cerr << "File: " << file_name << " does not exist!\n";
		return 1;
	}

	// Use C style for performance
	FILE * file = fopen( file_name.c_str(), "r" );
	fseek( file, 0, SEEK_END );
	size_t size = ftell( file );
	rewind( file );

	char * ptr_data = new char[ size + 1 ];
	fread( ptr_data, sizeof( char ), size, file );
	ptr_data[ size ] = '\0';

	fclose( file );
	std::string data( ptr_data );
	delete[] ptr_data;

	return data;
}

bool FS::CreateDirectoriesForFile( const std::string & file )
{
	auto dir = Env::GetDirPart( file );
	if( Env::Exec( "mkdir -p " + dir ) != 0 ) {
		std::cerr << "CreateDirectoriesForFile() failed for location: " << dir << "\n";
		return false;
	}
	return true;
}