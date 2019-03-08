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
#include "../../include/FS.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

AL_FUNC_FIX_ARG( play, 2, false, false )
{
	std::string music_addr;
	EVAL_AND_CHECK( "play", args[ 0 ], music_addr );
	Env::SetVar( music_addr, "0" );

	std::string music_file;
	EVAL_AND_CHECK( "play", args[ 1 ], music_file );

	if( !FS::LocExists( music_file ) ) {
		std::cerr << "libaudio: Audio file: " << music_file << " not found\n";
		return FAIL;
	}
	sf::Music * m = new sf::Music();
	auto m_loc = std::to_string( ( unsigned long long )m );
	if( m == nullptr ) {
		std::cerr << "libaudio: Failed to create sf::Music to location: " << m_loc << "\n";
		return FAIL;
	}

	if( !m->openFromFile( music_file ) ) {
		std::cerr << "libaudio: Failed to open music file: " << music_file << "\n";
		return FAIL;
	}
	Env::SetVar( music_addr, m_loc );
	m->play();
	return OK;
}

AL_FUNC_FIX_ARG( pause, 1, false, false )
{
	std::string music_addr;
	EVAL_AND_CHECK( "pause", args[ 0 ], music_addr );
	auto m_loc = Env::GetVar( music_addr );
	sf::Music * m = ( sf::Music * ) std::stoull( m_loc );
	if( m == nullptr ) {
		std::cerr << "libaudio: Failed to fetch sf::Music from location: " << m_loc << "\n";
		return FAIL;
	}
	m->pause();
	return OK;
}

AL_FUNC_FIX_ARG( resume, 1, false, false )
{
	std::string music_addr;
	EVAL_AND_CHECK( "resume", args[ 0 ], music_addr );
	auto m_loc = Env::GetVar( music_addr );
	sf::Music * m = ( sf::Music * ) std::stoull( m_loc );
	if( m == nullptr ) {
		std::cerr << "libaudio: Failed to fetch sf::Music from location: " << m_loc << "\n";
		return FAIL;
	}
	m->play();
	return OK;
}

AL_FUNC_FIX_ARG( stop, 1, false, false )
{
	std::string music_addr;
	EVAL_AND_CHECK( "stop", args[ 0 ], music_addr );
	auto m_loc = Env::GetVar( music_addr );
	sf::Music * m = ( sf::Music * ) std::stoull( m_loc );
	if( m == nullptr ) {
		std::cerr << "libaudio: Failed to fetch sf::Music from location: " << m_loc << "\n";
		return FAIL;
	}
	m->stop();
	delete m;
	Env::Reset( music_addr );
	return OK;
}

AL_FUNC_FIX_ARG( setvol, 2, false, false )
{
	std::string music_addr;
	EVAL_AND_CHECK( "setvol", args[ 0 ], music_addr );
	auto m_loc = Env::GetVar( music_addr );
	sf::Music * m = ( sf::Music * ) std::stoull( m_loc );
	if( m == nullptr ) {
		std::cerr << "libaudio: Failed to fetch sf::Music from location: " << m_loc << "\n";
		return FAIL;
	}
	std::string vol_str;
	EVAL_AND_CHECK( "setvol", args[ 1 ], vol_str );
	CHECK_VAR_NUMERIC( args[ 1 ], vol_str );
	m->setVolume( std::stoi( vol_str ) % 100 );
	return OK;
}