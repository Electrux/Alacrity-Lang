/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef STRING_HPP
#define STRING_HPP

#include <vector>
#include <string>
#include <unordered_map>

namespace Str
{
bool IsNum( const std::string & str );
int Eval( const std::string & str, std::string & op );
void RemoveBackslash( std::string & s );

std::vector< std::string > DelimitOnce( const std::string & str, const char delim );
std::vector< std::string > Delimit( const std::string & str, const char ch );

std::unordered_map< std::string, std::string > GenEnvMap( std::string env );

void RemoveAllPathsWithRegex( std::vector< std::string > & vec, const std::string & regex_str,
	const std::vector< std::string > & except = {} );
}

#endif // STRING_HPP