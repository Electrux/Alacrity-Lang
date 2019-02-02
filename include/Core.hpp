/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef CORE_HPP
#define CORE_HPP

#include <vector>
#include <string>

namespace Core
{
/* Find an element in vector by index */
template< typename T >
int LocInVec( const std::vector< T > & vec, const T & element )
{
	for( int i = 0; i < vec.size(); ++i ) {
		if( vec[ i ] == element ) return i;
	}
	return -1;
}

/* Core defaults/static variables wrapped in functions */
constexpr char PathDelim = ':';
std::string ALLibPaths();
std::string ALSourcePaths();
std::string FuncLibFile( const std::string & func_name );

/* Required for Interpreter */
int Init();
void DeInit();
}

#endif // CORE_HPP