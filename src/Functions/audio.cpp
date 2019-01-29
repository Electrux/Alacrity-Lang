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
#include <SFML/Audio.hpp>

#include "../../include/Errors.hpp"
#include "../../include/Core.hpp"
#include "../../include/String.hpp"
#include "../../include/Env.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

AL_FUNC_FIX_ARG( init, 1, false, false )
{
	std::string addr;
	EVAL_AND_CHECK( "init", args[ 0 ], addr );
	Env::SetVar( addr, "0" );
	sf::Sound * s = new sf::Sound();
	if( s == nullptr ) {
		std::cerr << "libaudio: Failed to create sf::Sound\n";
		return FAIL;
	}
	std::string loc = std::to_string( ( unsigned long long )s );
	Env::SetVar( addr, loc );
	return OK;
}

AL_FUNC_FIX_ARG( deinit, 1, false, false )
{
	std::string addr;
	EVAL_AND_CHECK( "deinit", args[ 0 ], addr );
	auto loc = Env::GetVar( addr );
	if( !Str::IsNum( loc ) ) {
		std::cerr << "libaudio: Invalid handle provided, contains malformed data\n";
		return FAIL;
	}
	sf::Sound * s = ( sf::Sound * ) std::stoull( loc );
	if( s == nullptr ) {
		std::cerr << "libaudio: Failed to fetch sf::Sound from location: " << loc << "\n";
		return FAIL;
	}
	delete s;
	return OK;
}

AL_FUNC_FIX_ARG( play, 3, false, false )
{
	std::string sound_addr, music_addr;
	EVAL_AND_CHECK( "play", args[ 0 ], sound_addr );
	EVAL_AND_CHECK( "play", args[ 1 ], music_addr );
	auto loc = Env::GetVar( sound_addr );
	if( !Str::IsNum( loc ) ) {
		std::cerr << "libaudio: Invalid handle provided, contains malformed data\n";
		return FAIL;
	}
	sf::Sound * s = ( sf::Sound * ) std::stoull( loc );
	if( s == nullptr ) {
		std::cerr << "libaudio: Failed to fetch sf::Sound from location: " << loc << "\n";
		return FAIL;
	}
	Env::SetVar( music_addr, "0" );
	sf::Music * m = new sf::Music();
	auto m_loc = std::to_string( ( unsigned long long )m );
	if( m == nullptr ) {
		std::cerr << "libaudio: Failed to create sf::Music to location: " << m_loc << "\n";
		return FAIL;
	}

	if( !m->openFromFile( args[ 2 ] ) ) {
		std::cerr << "libaudio: Failed to open music file: " << args[ 2 ] << "\n";
		return FAIL;
	}
	Env::SetVar( music_addr, m_loc );
	m->play();
	return OK;
}

AL_FUNC_FIX_ARG( stop, 2, false, false )
{
	std::string sound_addr, music_addr;
	EVAL_AND_CHECK( "stop", args[ 0 ], sound_addr );
	EVAL_AND_CHECK( "stop", args[ 1 ], music_addr );
	auto loc = Env::GetVar( sound_addr );
	if( !Str::IsNum( loc ) ) {
		std::cerr << "libaudio: Invalid handle provided, contains malformed data\n";
		return FAIL;
	}
	sf::Sound * s = ( sf::Sound * ) std::stoull( loc );
	if( s == nullptr ) {
		std::cerr << "libaudio: Failed to fetch sf::Sound from location: " << loc << "\n";
		return FAIL;
	}
	auto m_loc = Env::GetVar( music_addr );
	sf::Music * m = ( sf::Music * ) std::stoull( m_loc );
	if( m == nullptr ) {
		std::cerr << "libaudio: Failed to fetch sf::Music from location: " << m_loc << "\n";
		return FAIL;
	}
	m->stop();
	delete m;
	return OK;
}