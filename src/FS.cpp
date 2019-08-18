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

#include <regex>
#include <variant>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "../include/Env.hpp"
#include "../include/FS.hpp"

#define MAX_PATH 1024

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
	char cwd[ MAX_PATH ];
	if( getcwd( cwd, sizeof( cwd ) ) != NULL ) {
		return cwd;
	}
	return "";
}

void FS::SetCurrentDir( const std::string & path )
{
	chdir( path.c_str() );
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

std::string FS::GetFilePath( const std::string & file, const std::string & env_var, const char delim )
{
	std::string file_path = file;
	if( file_path[ 0 ] == '~' ) {
		file_path.erase( file_path.begin() );
		file_path = Env::Home() + "/" + file_path;
	}
	if( file_path[ 0 ] != '.' && file_path[ 0 ] != '/' ) {
		file_path = FS::GetCurrentDir() + "/" + file_path;
	}

	if( FS::LocExists( file_path ) ) return file_path;

	if( env_var.empty() ) {
		std::cout << "File: " << file_path << " (derived from: " << file << ") does not exist!\n";
		return "";
	}
	auto final_path = Env::GetFileLocation( Env::GetVar( env_var ), file, delim );
	if( final_path.empty() ) {
		std::cout << "File: " << file << " could not be found in the list!\n";
	}
	return final_path;
}

void get_entries_internal( const std::string & dir_str, std::vector< std::string > & v, const std::regex & regex, const bool rev_regex )
{
	DIR * dir;
	struct dirent * ent;
	if( ( dir = opendir( dir_str.c_str() ) ) == NULL ) return;

	while( ( ent = readdir( dir ) ) != NULL ) {
		if( strcmp( ent->d_name, "." ) == 0 || strcmp( ent->d_name, ".." ) == 0 ) continue;
		if( ent->d_type == DT_DIR ) {
			get_entries_internal( dir_str + ent->d_name + "/", v, regex, rev_regex );
			continue;
		}
		std::string loc = dir_str + ent->d_name;
		if( rev_regex ) {
			if( !std::regex_match( loc, regex ) ) v.push_back( loc );
		} else {
			if( std::regex_match( loc, regex ) ) v.push_back( loc );
		}
	}
	closedir( dir );
}

std::vector< std::string > get_entries( std::string dir_str, const std::regex & regex, const bool rev_regex )
{
	std::vector< std::string > v;
	if( dir_str.size() > 0 && dir_str.back() != '/' ) dir_str += "/";
	get_entries_internal( dir_str, v, regex, rev_regex );
	return v;
}

std::vector< std::string > FS::GetFilesFromRegex( std::string regex_str )
{
	if( regex_str.empty() ) return {};
	bool reverse_regex = false;

	if( * regex_str.begin() == '-' ) {
		reverse_regex = true;
		regex_str.erase( regex_str.begin() );
	}

	if( regex_str.size() > 0 && * regex_str.begin() == '.' ) {
		regex_str.erase( regex_str.begin() );
		if( regex_str.size() > 0 && * regex_str.begin() == '/' ) regex_str.erase( regex_str.begin() );
	}
	if( regex_str.size() == 0 ) return {};

	auto src_dir = regex_str.substr( 0, regex_str.find_last_of( '/' ) + 1 );
	if( src_dir.empty() ) src_dir = "./";

	std::regex regex( regex_str );
	std::vector< std::string > res = get_entries( src_dir, regex, reverse_regex );
	return res;
}

bool FS::DeleteFile( const std::string & file )
{
	return std::remove( file.c_str() ) == 0 ? true : false;
}