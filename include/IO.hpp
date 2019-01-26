/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef IO_HPP
#define IO_HPP

#include <iostream>
#include <vector>
#include <unordered_map>

namespace IO
{
	class Out
	{
		// bool signifies, to draw or not
		std::vector< bool > m_tabs;
		void Tab( const bool has_next ) const;
	public:
		Out();
		const Out & operator() ( const bool has_next = false ) const;
		const Out & operator<< ( const std::string & val ) const;
		const Out & operator<< ( const bool val ) const;
		const Out & operator<< ( const char * val ) const;
		const Out & operator<< ( const int val ) const;
		const Out & operator<< ( const long val ) const;
		const Out & operator<< ( const long long val ) const;
		const Out & operator<< ( const float val ) const;
		const Out & operator<< ( const double val ) const;
		const Out & operator<< ( const short val ) const;
		const Out & operator<< ( const void * val ) const;

		void IncTab( const bool draw );
		void DecTab( const int tabs = 1 );

		int GetTabs() const;
	};

	extern Out out;

	extern std::unordered_map< std::string, std::string > COLORS;

	class ColOut
	{
		bool m_enable;
		bool m_use_colors;
		bool m_eval;
	public:
		ColOut();
		void Enable( const bool enable );
		bool IsEnabled();
		void UseColors( const bool use );
		void SetEval( const bool eval );
		bool IsEval();
		const ColOut & operator() ( const bool eval = false );
		const ColOut & operator<< ( std::string val ) const;
		const ColOut & operator<< ( const bool val ) const;
		const ColOut & operator<< ( const char * val ) const;
		const ColOut & operator<< ( const int val ) const;
		const ColOut & operator<< ( const long val ) const;
		const ColOut & operator<< ( const unsigned long val ) const;
		const ColOut & operator<< ( const long long val ) const;
		const ColOut & operator<< ( const float val ) const;
		const ColOut & operator<< ( const double val ) const;
		const ColOut & operator<< ( const short val ) const;
		const ColOut & operator<< ( const void * val ) const;

		static void ApplyColors( std::string & str );
	};

	extern ColOut colout;
}

#endif // IO_HPP
