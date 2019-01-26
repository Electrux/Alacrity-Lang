/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef LEXER_LEXER_HPP
#define LEXER_LEXER_HPP

#include <vector>
#include <string>
#include <variant>

#include "../Core.hpp"

namespace Lex
{
	enum SymType
	{
		NUM,
		STR,
		OPER,
		SEPAR,

		FSMLAST,

		// NOT FOR FSM

		// STR
		KEYW,

		// OPER
		LOGIC,
	};

	enum StrKeyw
	{
		IF,
		ELIF,
		ELSE,

		FOREACH,
		FOREACH_IN_VAR,

		CONCAT,
	};

	enum OperLogic
	{
		AND,
		OR,
		NOT,

		EQ,
		LT,
		GT,
		LE,
		GE,
		NE,
	};

	enum Separ
	{
		SPC,
		TAB,
		NEWL,
		COMMA,
		COLON,
		SEMICOL,

		PARENOP,
		PARENCL,
		BRACEOP,
		BRACECL,
		BRACKOP,
		BRACKCL,

		COMMENT,

		SINGQUOTE,
		DOUBQUOTE,
	};

	extern const std::vector< std::string > SymTypeStrs;
	extern const std::vector< std::string > KeywStrs;
	extern const std::vector< std::string > LogicStrs;
	extern const std::vector< char > SeparChars;

	class Sym
	{
		SymType m_type;
		std::string m_data;
		int m_detail_type;
		int m_line;
		int m_col;
	public:
		Sym( const SymType type, const int detail_type, const std::string & data, const int line, const int col );

		SymType GetType() const;
		const std::string & GetData() const;
		int GetDetailType() const;
		int GetLine() const;
		int GetCol() const;
	};

	// Will move the original data when constructed from other Syms
	class Syms
	{
		std::vector< Sym * > m_syms;
	public:
		Syms();
		Syms( Syms && original );
		Syms( Syms & original );
		Syms( Syms const & original ) = delete;
		Syms & operator =( Syms original );

		friend void swap( Syms & syms1, Syms & syms2 );

		Sym * operator []( const size_t loc );

		void push_back( Sym * const sym );
		void pop_back();
		size_t size() const;

		// iteration
		const std::vector< Sym * > & GetSyms() const;

		~Syms();
	};

	void swap( Syms & sym1, Syms & sym2 );

	std::variant< int, Syms > Tokenize( const std::string & data );

	void PrettyPrint( const Syms & syms );
}

typedef std::vector< Lex::Sym * > LexSymVec;

#endif // LEXER_LEXER_HPP
