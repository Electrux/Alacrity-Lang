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
#include <regex>

#include "../include/Errors.hpp"
#include "../include/Core.hpp"
#include "../include/Env.hpp"
#include "../include/IO.hpp"
#include "../include/String.hpp"

// $ -> Variable
// % -> Command output
// # -> Command result

static int GetInBraces( std::string::iterator & it, std::string & op, const std::string & orig,
			std::string & res, const bool colon = false, int * int_val = nullptr );

bool Str::IsNum( const std::string & str )
{
	if( str.empty() || ( ( !isdigit( str[ 0 ] ) ) && ( str[ 0 ] != '-' ) && ( str[ 0 ] != '+' ) ) ) return false;

	char * p;
	strtol( str.c_str(), & p, 10 );

	return ( * p == 0 );
}

int Str::Eval( const std::string & str, std::string & op )
{
	int res = OK;
	op = str;
	for( auto it = op.begin(); it != op.end(); ) {
		char ch = * it;
		char nextch = ( it + 1 < op.end() ) ? * ( it + 1 ) : -1;

		// Replace ~ with $HOME
		if( ch == '~' ) {
			if( it > op.begin() && * ( it - 1 ) == '\\' ) {
				++it;
				continue;
			}
			it = op.erase( it );
			std::string val = Env::Home();
			it = op.insert( it, val.begin(), val.end() );
			it += val.size();
			continue;
		}

		// Get variable or command output or command result
		if( ( ch == '$' || ch == '%' || ch == '#' ) && nextch == '{' ) {
			char type = ch;
			if( it > op.begin() && * ( it - 1 ) == '\\' ) {
				++it;
				continue;
			}
			it = op.erase( it );
			it = op.erase( it );
			int item_loc = -1;
			std::string source;
			res = GetInBraces( it, op, str, source, type == '%', & item_loc );
			if( res != OK ) {
				std::cout << "Str::Eval error: Failed GetInBraces() for string: " << str << "\n";
				break;
			}
			// remove ending brace
			it = op.erase( it );
			std::string val;
			if( type == '$' ) {
				val = Env::GetVar( source );
				res = OK;
			} else if( type == '%' ) {
				if( item_loc == -1 ) {
					res = Env::Exec( source, & val );
				} else {
					std::vector< std::string > out;
					res = Env::ExecVector( source, & out );
					if( res != 0 ) {
						std::cout << "Str::Eval error: Command: " << source << " failed!\n";
						break;
					}
					if( out.size() <= item_loc ) {
						std::cout << "Str::Eval error: The result of command: " << source << " contained only: " << out.size()
							<< " lines, but the given item location is: " << item_loc << "\n";
						res = IDX_OUT_BOUNDS;
						break;
					}
					val = out[ item_loc ];
				}
				if( !val.empty() && * ( val.end() - 1 ) == '\n' ) {
					val.erase( val.end() - 1 );
				}
				if( res != 0 ) {
					std::cout << "Str::Eval error: Command: " << source << " failed!\n";
					break;
				}
			} else if( type == '#' ) {
				res = Env::Exec( source );
				val = std::to_string( res );
				res = OK;
			}
			it = op.insert( it, val.begin(), val.end() );
			it += val.size();
			continue;
		}

		++it;
	}
	IO::ColOut::ApplyColors( op );
	if( res != OK ) {
		op.clear();
	}
	return res;
}

void Str::RemoveBackslash( std::string & s )
{
	for( auto it = s.begin(); it != s.end(); ++it ) {
		if( * it == '\\' ) {
			if( it + 1 >= s.end() ) continue;
			it = s.erase( it );
			if( * it == 'n' ) * it = '\n';
			else if( * it == 'r' ) * it = '\r';
			else if( * it == 't' ) * it = '\t';
		}
	}
}

static int GetInBraces( std::string::iterator & it, std::string & op, const std::string & orig,
			std::string & res, const bool colon, int * int_val )
{
	std::string int_res;
	bool colon_occur = false;
	int brace_count = 1;
	while( it < op.end() && brace_count > 0 ) {
		if( colon && brace_count == 1 && * it == ':' ) {
			colon_occur = true;
			it = op.erase( it );
			continue;
		}
		if( * it == '{' ) {
			++brace_count;
		} else if( * it == '}' ) {
			--brace_count;
			if( brace_count == 0 ) {
				continue;
			}
		}
		if( colon && colon_occur ) int_res += * it;
		else res += * it;
		it = op.erase( it );
	}
	if( brace_count > 0 ) {
		std::cout << "Str::GetInBraces error: String: " << orig << " contains invalid arrangement of braces (count: " << brace_count
			<< " (it should be 0))\n";
		return INVALID_STR;
	}
	if( colon ) {
		if( colon_occur ) {
			if( int_res.empty() ) {
				* int_val = 0;
			} else {
				* int_val = std::stoi( int_res );
			}
		} else {
			* int_val = -1;
		}
		
	}
	return OK;
}

std::vector< std::string > Str::DelimitOnce( const std::string & str, const char delim )
{
	std::vector< std::string > vec;
	std::string tmp;
	auto pos = str.find( delim );
	if( pos == std::string::npos ) return { str };
	return { str.substr( 0, pos ), str.substr( pos + 1 ) };
}

std::vector< std::string > Str::Delimit( const std::string & str, const char ch )
{
	std::string temp;
	std::vector< std::string > vec;

	for( auto c : str ) {
		if( c == ch ) {
			if( temp.empty() ) continue;
			vec.push_back( temp );
			temp.clear();
			continue;
		}

		temp += c;
	}

	if( !temp.empty() ) vec.push_back( temp );
	return vec;
}

std::unordered_map< std::string, std::string > Str::GenEnvMap( std::string env )
{
	std::unordered_map< std::string, std::string > env_map;
	auto line_pos = env.find( '\n' );
	while( ( line_pos = env.find( '\n' ) ) != std::string::npos ) {
		auto res = DelimitOnce( env.substr( 0, line_pos ), '=' );
		env_map[ res[ 0 ] ] = res[ 1 ];
		env = env.substr( line_pos + 1 );
	}
	if( env.find( '=' ) != std::string::npos ) {
		auto res = DelimitOnce( env, '=' );
		env_map[ res[ 0 ] ] = res[ 1 ];
	}
	return env_map;
}

void Str::RemoveAllPathsWithRegex( std::vector< std::string > & vec, const std::string & regex_str,
	const std::vector< std::string > & except )
{
	if( regex_str.empty() ) return;
	std::regex regex( Env::GetExactRegexPath( regex_str ) );
	for( auto it = vec.begin(); it != vec.end(); ) {
		if( std::regex_match( * it, regex ) && Core::LocInVec( except, * it ) == -1 ) it = vec.erase( it );
		else ++it;
	}
}