/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef ENV_HPP
#define ENV_HPP

#include <vector>
#include <string>
#include <unordered_map>

namespace Env
{
/* Manipulate Environment Variables */
std::string GetVar( const std::string & var );
bool SetVar( const std::string & var, const std::string & val );

/* SHELL and HOME environment variables (static initialization) */
std::string Shell();
std::string Home();

/* Manage backing up and restoring all the environment variables */
std::vector< std::unordered_map< std::string, std::string > > & Backups();
void Backup();
bool Restore();

/* Process execution functions, ExecVector stores each output line as an element in vector: vec */
int ExecVector( const std::string & cmd, std::vector< std::string > * vec = nullptr );
int Exec( const std::string & cmd, std::string * out = nullptr );

/* Wrap a command with $SHELL -c "<command>" */
std::string WrapShell( const std::string & cmd );

/* Fetch the directory portion and file portion from a full path */
std::string GetDirPart( const std::string & full_loc );
std::string GetFilePart( const std::string & full_loc );

/* Check if file exists in all locations stored in paths, separated by delim */
std::string GetFileLocation( const std::string & paths, const std::string & file, const char delim );

/* Manage environment variables */
void Append( const std::string & var, const std::string & val, const char delim = ':' );
void Prepend( const std::string & var, const std::string & val, const char delim = ':' );
void Reset( const std::string & var );
void Remove( const std::string & var, const std::string & val, const char delim = ':' );

/* Prepend current directory to loc */
std::string AddCurrDir( std::string loc );

/* Fetch full path from regex string str */
std::string GetExactRegexPath( const std::string & str );
}

#endif // ENV_HPP