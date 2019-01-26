/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef DYN_LIB_HPP
#define DYN_LIB_HPP

#include <string>
#include <unordered_map>

/* Wrapper class for dlfcn.h library */
class DynLib
{
	static DynLib * m_dyn_lib;
	std::unordered_map< std::string, void * > m_handles;
	DynLib();
	~DynLib();
public:
	static DynLib * Get();
	void * Load( const std::string & file );
	void Unload( const std::string & file );
	void * GetSym( const std::string & file, const std::string & sym );
	static void Deinit();
};

#endif // DYN_LIB_HPP