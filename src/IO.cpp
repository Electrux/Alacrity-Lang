/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <iostream>
#include <unordered_map>

#include "../include/IO.hpp"
#include "../include/String.hpp"

using namespace IO;

IO::Out::Out() : m_tabs( 0 )
{}

void IO::Out::Tab( const bool has_next ) const
{
	for( int i = 0; i < m_tabs.size(); ++i ) {
		if( i == m_tabs.size() - 1 ) {
			if( has_next ) std::cout << "  ├─";
			else std::cout << "  └─";
		}
		else {
			if( m_tabs[ i ] ) std::cout << "  │";
			else std::cout << "   ";
		}
	}
}

const Out & IO::Out::operator() ( const bool has_next ) const
{
	Tab( has_next );
	return * this;
}

const Out & IO::Out::operator<< ( const std::string & val ) const
{
	std::cout << val;
	return * this;
}

const Out & IO::Out::operator<< ( const bool val ) const
{
	std::cout << std::boolalpha << val;
	return * this;
}

const Out & IO::Out::operator<< ( const char * val ) const
{
	std::cout << val;
	return * this;
}

const Out & IO::Out::operator<< ( const int val ) const
{
	std::cout << val;
	return * this;
}

const Out & IO::Out::operator<< ( const long val ) const
{
	std::cout << val;
	return * this;
}

const Out & IO::Out::operator<< ( const long long val ) const
{
	std::cout << val;
	return * this;
}

const Out & IO::Out::operator<< ( const float val ) const
{
	std::cout << val;
	return * this;
}

const Out & IO::Out::operator<< ( const double val ) const
{
	std::cout << val;
	return * this;
}

const Out & IO::Out::operator<< ( const short val ) const
{
	std::cout << val;
	return * this;
}

const Out & IO::Out::operator<< ( const void * val ) const
{
	std::cout << val;
	return * this;
}

void IO::Out::IncTab( const bool draw )
{
	m_tabs.push_back( draw );
}

void IO::Out::DecTab( const int tabs )
{
	if( m_tabs.size() < tabs ) return;
	for( int i = 0; i < tabs; ++i ) {
		m_tabs.pop_back();
	}
}

int IO::Out::GetTabs() const
{
	return m_tabs.size();
}

IO::Out IO::out;

std::unordered_map< std::string, std::string > IO::COLORS = {
	{ "0", "\033[0m" },

	{ "r", "\033[0;31m" },
	{ "g", "\033[0;32m" },
	{ "y", "\033[0;33m" },
	{ "b", "\033[0;34m" },
	{ "m", "\033[0;35m" },
	{ "c", "\033[0;36m" },
	{ "w", "\033[0;37m" },

	{ "br", "\033[1;31m" },
	{ "bg", "\033[1;32m" },
	{ "by", "\033[1;33m" },
	{ "bb", "\033[1;34m" },
	{ "bm", "\033[1;35m" },
	{ "bc", "\033[1;36m" },
	{ "bw", "\033[1;37m" },

	{ "fc", "\033[0;35m" },
	{ "sc", "\033[0;33m" },
	{ "tc", "\033[0;37m" },
	{ "lc", "\033[0;36m" },
};

IO::ColOut::ColOut() : m_enable( true ), m_use_colors( true ), m_eval( false ) {}

void IO::ColOut::Enable( const bool enable ) { m_enable = enable; }
bool IO::ColOut::IsEnabled() { return m_enable; }

void IO::ColOut::UseColors( const bool use ) { m_use_colors = use; }

void IO::ColOut::SetEval( const bool eval ) { m_eval = eval; }
bool IO::ColOut::IsEval() { return m_eval; }

const ColOut & IO::ColOut::operator() ( const bool eval )
{
	m_eval = eval;
	return * this;
}

const ColOut & IO::ColOut::operator<< ( std::string val ) const
{
	if( m_enable ) {
		std::string op;
		if( m_use_colors && m_eval ) {
			Str::Eval( val, op );
		} else if( m_use_colors ) {
			ApplyColors( val );
			op = val;
		} else {
			op = val;
		}
		std::cout << op;
	}
	return * this;
}

const ColOut & IO::ColOut::operator<< ( const bool val ) const
{
	if( m_enable ) {
		if( m_use_colors ) {
			if( val ) std::cout << COLORS[ "bg" ];
			else std::cout << COLORS[ "br" ];
		}
		std::cout << std::boolalpha << val;
		if( m_use_colors ) std::cout << COLORS[ "0" ];
	}
	return * this;
}

const ColOut & IO::ColOut::operator<< ( const char * val ) const
{
	( * this ) << std::string( val );
	return * this;
}

const ColOut & IO::ColOut::operator<< ( const int val ) const
{
	if( m_enable ) {
		std::cout << val;
	}
	return * this;
}

const ColOut & IO::ColOut::operator<< ( const long val ) const
{
	if( m_enable ) {
		if( m_use_colors ) std::cout << COLORS[ "by" ];
		std::cout << val;
		if( m_use_colors ) std::cout << COLORS[ "0" ];
	}
	return * this;
}

const ColOut & IO::ColOut::operator<< ( const unsigned long val ) const
{
	if( m_enable ) {
		if( m_use_colors ) std::cout << COLORS[ "by" ];
		std::cout << val;
		if( m_use_colors ) std::cout << COLORS[ "0" ];
	}
	return * this;
}

const ColOut & IO::ColOut::operator<< ( const long long val ) const
{
	if( m_enable ) {
		if( m_use_colors ) std::cout << COLORS[ "by" ];
		std::cout << val;
		if( m_use_colors ) std::cout << COLORS[ "0" ];
	}
	return * this;
}

const ColOut & IO::ColOut::operator<< ( const float val ) const
{
	if( m_enable ) {
		if( m_use_colors ) std::cout << COLORS[ "by" ];
		std::cout << val;
		if( m_use_colors ) std::cout << COLORS[ "0" ];
	}
	return * this;
}

const ColOut & IO::ColOut::operator<< ( const double val ) const
{
	if( m_enable ) {
		if( m_use_colors ) std::cout << COLORS[ "by" ];
		std::cout << val;
		if( m_use_colors ) std::cout << COLORS[ "0" ];
	}
	return * this;
}

const ColOut & IO::ColOut::operator<< ( const short val ) const
{
	if( m_enable ) {
		if( m_use_colors ) std::cout << COLORS[ "by" ];
		std::cout << val;
		if( m_use_colors ) std::cout << COLORS[ "0" ];
	}
	return * this;
}

const ColOut & IO::ColOut::operator<< ( const void * val ) const
{
	if( m_enable ) {
		if( m_use_colors ) std::cout << COLORS[ "bb" ];
		std::cout << val;
		if( m_use_colors ) std::cout << COLORS[ "0" ];
	}
	return * this;
}

void IO::ColOut::ApplyColors( std::string & str )
{
	for( auto it = str.begin(); it != str.end(); ) {
		if( * it == '{' && ( it == str.begin() || ( * ( it - 1 ) != '$' && * ( it - 1 ) != '%' && * ( it - 1 ) != '#' ) ) ) {
			it = str.erase( it );
			if( it != str.end() && * it == '{' ) {
				++it;
				continue;
			}

			std::string var;

			while( it != str.end() && * it != '}' ) {
				var += * it;
				it = str.erase( it );
			}

			// Remove the ending brace
			if( it != str.end() ) it = str.erase( it );

			if( var.empty() ) continue;

			std::string val = IO::COLORS[ var ];

			it = str.insert( it, val.begin(), val.end() );
			it += val.size();
		}
		else {
			++it;
		}
	}
}

IO::ColOut IO::colout;