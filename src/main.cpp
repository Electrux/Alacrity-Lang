/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.

*/

#include <iostream>
#include <string>
#include <variant>

#include "../include/Errors.hpp"
#include "../include/Env.hpp"
#include "../include/FS.hpp"
#include "../include/Lexer/Lexer.hpp"
#include "../include/Parser.hpp"
#include "../include/Interpreter.hpp"

int main( int argc, char ** argv )
{
	if( argc < 2 ) {
		std::cerr << "Error: No source file provided!\n";
		return -1;
	}
	auto file_data_var = FS::ReadFile( argv[ 1 ] );
	if( std::holds_alternative< int >( file_data_var ) ) return std::get< int >( file_data_var );
	std::string file_data = std::get< std::string >( file_data_var );

	auto lex_syms_var = Lex::Tokenize( file_data );

	if( std::holds_alternative< int >( lex_syms_var ) ) return std::get< int >( lex_syms_var );
	auto lex_syms = std::get< Lex::Syms >( lex_syms_var );

	/*
	std::cout << "Lexical pretty print:\n";
	Lex::PrettyPrint( lex_syms );
	*/
	/*
	std::cout << "Lexical symbols:\n";
	for( const auto & sym : lex_syms.GetSyms() ) {
		std::cout << "Type: " << Lex::SymTypeStrs[ sym->GetType() ]
			<< "\tDetail Type: " << sym->GetDetailType()
			<< "\tLocation: " << sym->GetLine() << "[" << sym->GetCol() << "]"
			<< "\tValue: " << sym->GetData() << "\n";
	}
	*/

	auto parse_syms_var = Parser::ParseTokens( lex_syms.GetSyms() );
	if( std::holds_alternative< int >( parse_syms_var ) ) return std::get< int >( parse_syms_var );
	auto parse_syms = std::get< Parser::ParseTree >( parse_syms_var );

	/*
	std::cout << "\nParse Tree:\n";
	for( int i = 0; i < parse_syms.GetStmts().size(); ++i ) {
		parse_syms.GetStmts()[ i ]->Disp( i != parse_syms.GetStmts().size() - 1 );
	}
	std::cout << "\n\n";
	*/

	int res = OK;
	res = Core::Init();
	if( res != OK ) return res;

	std::string file_path = argv[ 1 ];
	if( file_path[ 0 ] == '~' ) {
		file_path.erase( file_path.begin() );
		file_path = Env::Home() + "/" + file_path;
	}
	if( file_path[ 0 ] != '.' && file_path[ 0 ] != '/' ) {
		file_path = FS::GetCurrentDir() + "/" + file_path;
	}

	if( !FS::LocExists( file_path ) ) {
		std::cout << "File: " << file_path << " (derived from: " << argv[ 1 ] << ") does not exist!\n";
		return FILE_NOT_FOUND;
	}

	return Interpreter::Interpret( parse_syms, file_path, 0, false );
}
