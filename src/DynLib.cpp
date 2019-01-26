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
#include <unordered_map>
#include <dlfcn.h>

#include "../include/DynLib.hpp"

DynLib * DynLib::m_dyn_lib = nullptr;

DynLib::DynLib() {}

DynLib::~DynLib()
{
	for( auto & e : m_handles ) {
		if( e.second != nullptr ) dlclose( e.second );
	}
}

DynLib * DynLib::Get()
{
	if( m_dyn_lib == nullptr ) {
		m_dyn_lib = new DynLib();
	}
	return m_dyn_lib;
}

void * DynLib::Load( const std::string & file )
{
	if( m_handles.find( file ) == m_handles.end() ) {
		auto tmp = dlopen( file.c_str(), RTLD_LAZY );
		if( tmp == nullptr ) {
			std::cout << "Dyn Lib error: " << dlerror() << "\n";
			return nullptr;
		}
		m_handles[ file ] = tmp;
	}
	return m_handles[ file ];
}

void DynLib::Unload( const std::string & file )
{
	if( m_handles.find( file ) == m_handles.end() ) return;
	dlclose( m_handles[ file ] );
	m_handles.erase( file );
}

void * DynLib::GetSym( const std::string & file, const std::string & sym )
{
	if( m_handles.find( file ) == m_handles.end() ) return nullptr;
	return dlsym( m_handles[ file ], sym.c_str() );
}


void DynLib::Deinit() { if( m_dyn_lib != nullptr ) { delete m_dyn_lib; m_dyn_lib = nullptr; } }