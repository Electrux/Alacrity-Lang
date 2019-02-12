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
#include <SFML/Network.hpp>

#include "../../include/Errors.hpp"
#include "../../include/Core.hpp"
#include "../../include/String.hpp"
#include "../../include/Env.hpp"
#include "../../include/FS.hpp"
#include "../../include/Interpreter/Block.hpp"
#include "../../include/Interpreter/FnBase.hpp"

AL_FUNC_FIX_ARG( local_ip, 1, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "local_ip", args[ 0 ], var );
	Env::SetVar( var, sf::IpAddress::getLocalAddress().toString() );
	return OK;
}

AL_FUNC_FIX_ARG( public_ip, 1, false, false )
{
	std::string var;
	EVAL_AND_CHECK( "public_ip", args[ 0 ], var );
	Env::SetVar( var, sf::IpAddress::getPublicAddress().toString() );
	return OK;
}

AL_FUNC_FIX_ARG( listen, 2, false, false )
{
	sf::TcpListener * listener = new sf::TcpListener;

	std::string port_str;
	EVAL_AND_CHECK( "listen", args[ 1 ], port_str );
	CHECK_VAR_NUMERIC( args[ 1 ], port_str );

	std::string listener_var;
	EVAL_AND_CHECK( "listen", args[ 0 ], listener_var );
	Env::SetVar( listener_var, std::to_string( ( unsigned long long )listener ) );

	listener->setBlocking( false );
	listener->listen( std::stoi( port_str ) );
	return OK;
}

AL_FUNC_FIX_ARG( unlisten, 1, false, false )
{
	std::string listener_loc;
	EVAL_AND_CHECK( "unlisten", args[ 0 ], listener_loc );
	listener_loc = Env::GetVar( listener_loc );
	CHECK_VAR_NUMERIC( args[ 0 ], listener_loc );
	sf::TcpListener * listener = ( sf::TcpListener * )std::stoull( listener_loc );
	if( listener == nullptr ) {
		std::cerr << "libnet: Failed to fetch TcpListener's location from: " << listener_loc << "\n";
		return FAIL;
	}
	listener->close();
	delete listener;
	return OK;
}

AL_FUNC_FIX_ARG( accept, 2, false, false )
{
	std::string listener_loc;
	EVAL_AND_CHECK( "accept", args[ 0 ], listener_loc );
	listener_loc = Env::GetVar( listener_loc );
	CHECK_VAR_NUMERIC( args[ 0 ], listener_loc );
	sf::TcpListener * listener = ( sf::TcpListener * )std::stoull( listener_loc );
	if( listener == nullptr ) {
		std::cerr << "libnet: Failed to fetch TcpListener's location from: " << listener_loc << "\n";
		return FAIL;
	}

	std::string socket_loc;
	EVAL_AND_CHECK( "accept", args[ 1 ], socket_loc );
	sf::TcpSocket * socket = new sf::TcpSocket;
	if( listener->accept( * socket ) == sf::Socket::Done ) {
		socket->setBlocking( false );
		Env::SetVar( socket_loc, std::to_string( ( unsigned long long )socket ) );
	} else {
		delete socket;
		Env::Reset( socket_loc );
	}
	return OK;
}

AL_FUNC_FIX_ARG( each_recv, 3, true, true )
{
	std::string socks_var;
	EVAL_AND_CHECK( "each_recv", args[ 0 ], socks_var );
	socks_var = Env::GetVar( socks_var );
	CHECK_VAR_NUMERIC( args[ 0 ], socks_var );
	std::vector< std::string > * socks_vec = ( std::vector< std::string > * )std::stoull( socks_var );
	if( socks_vec == nullptr ) {
		std::cerr << "libnet: Failed to fetch socket vector's location from: " << socks_var << "\n";
		return FAIL;
	}

	std::string sock_str, data;
	EVAL_AND_CHECK( "each_recv", args[ 1 ], sock_str );
	EVAL_AND_CHECK( "each_recv", args[ 2 ], data );

	int res = OK;
	for( auto it = socks_vec->begin(); it != socks_vec->end(); ) {
		auto & sock_loc = * it;
		Env::SetVar( sock_str, sock_loc );

		CHECK_VAR_NUMERIC( sock_loc, sock_loc );
		sf::TcpSocket * sock = ( sf::TcpSocket * )std::stoull( sock_loc );
		sf::Packet pack;
		if( sock->receive( pack ) == sf::Socket::Disconnected ) {
			delete sock;
			it = socks_vec->erase( it );
			continue;
		}
		if( sock->receive( pack ) != sf::Socket::Done ) continue;

		std::string tmp_data;
		pack >> tmp_data;
		Env::SetVar( data, tmp_data );
		res = Interpreter::Block( block, depth, internal_display_enabled );
		if( res != OK && res != LOOP_CONTINUE_ENCOUNTERED ) break;
		++it;
	}
	Env::Reset( sock_str );
	Env::Reset( data );
	return res;
}

AL_FUNC_FIX_ARG( connect, 3, false, false )
{
	std::string socket_loc;
	EVAL_AND_CHECK( "connect", args[ 0 ], socket_loc );
	sf::TcpSocket * sock = new sf::TcpSocket;

	std::string ip_str, port_str;
	EVAL_AND_CHECK( "connect", args[ 1 ], ip_str );
	EVAL_AND_CHECK( "connect", args[ 2 ], port_str );
	CHECK_VAR_NUMERIC( args[ 2 ], port_str );

	sf::IpAddress ip;
	if( ip_str == "localhost" || ip_str == "127.0.0.1" ) 	ip = sf::IpAddress::LocalHost;
	else							ip = ip_str;
	if( sock->connect( sf::IpAddress::LocalHost, std::stoi( port_str ) ) == sf::Socket::Done ) {
		Env::Reset( "RESULT" );
		//sock->setBlocking( false );
		Env::SetVar( socket_loc, std::to_string( ( unsigned long long )sock ) );
	} else {
		Env::SetVar( "RESULT", "FAIL" );
		delete sock;
	}
	return OK;
}

AL_FUNC_FIX_ARG( disconnect, 1, false, false )
{
	std::string socket_loc;
	EVAL_AND_CHECK( "disconnect", args[ 0 ], socket_loc );
	socket_loc = Env::GetVar( socket_loc );
	CHECK_VAR_NUMERIC( args[ 0 ], socket_loc );
	sf::TcpSocket * sock = ( sf::TcpSocket * )std::stoull( socket_loc );
	if( sock == nullptr ) {
		std::cerr << "libnet: Failed to fetch TcpSocket's location from: " << socket_loc << "\n";
		return FAIL;
	}

	sock->disconnect();
	delete sock;
	return OK;
}

AL_FUNC_FIX_ARG( disconnected, 2, false, false )
{
	std::string socket_loc;
	EVAL_AND_CHECK( "disconnected", args[ 0 ], socket_loc );
	socket_loc = Env::GetVar( socket_loc );
	CHECK_VAR_NUMERIC( args[ 0 ], socket_loc );
	sf::TcpSocket * sock = ( sf::TcpSocket * )std::stoull( socket_loc );
	if( sock == nullptr ) {
		std::cerr << "libnet: Failed to fetch TcpSocket's location from: " << socket_loc << "\n";
		return FAIL;
	}

	std::string var;
	EVAL_AND_CHECK( "disconnected", args[ 1 ], var );
	sf::Packet pack;
	if( sock->receive( pack ) == sf::Socket::Disconnected ) {
		delete sock;
		Env::SetVar( var, "true" );
	} else {
		Env::Reset( var );
	}

	return OK;
}

AL_FUNC_FIX_ARG( sock_ip, 2, false, false )
{
	std::string socket_loc;
	EVAL_AND_CHECK( "sock_ip", args[ 0 ], socket_loc );
	socket_loc = Env::GetVar( socket_loc );
	CHECK_VAR_NUMERIC( args[ 0 ], socket_loc );
	sf::TcpSocket * sock = ( sf::TcpSocket * )std::stoull( socket_loc );
	if( sock == nullptr ) {
		std::cerr << "libnet: Failed to fetch TcpSocket's location from: " << socket_loc << "\n";
		return FAIL;
	}

	std::string var;
	EVAL_AND_CHECK( "sock_ip", args[ 1 ], var );
	Env::SetVar( var, sock->getRemoteAddress().toString() );
	return OK;
}

AL_FUNC_FIX_ARG( send, 2, false, false )
{
	std::string socket_loc;
	EVAL_AND_CHECK( "send", args[ 0 ], socket_loc );
	socket_loc = Env::GetVar( socket_loc );
	CHECK_VAR_NUMERIC( args[ 0 ], socket_loc );
	sf::TcpSocket * sock = ( sf::TcpSocket * )std::stoull( socket_loc );
	if( sock == nullptr ) {
		std::cerr << "libnet: Failed to fetch TcpSocket's location from: " << socket_loc << "\n";
		return FAIL;
	}

	std::string data;
	EVAL_AND_CHECK( "send", args[ 1 ], data );
	sf::Packet pack;
	pack << data;
	if( sock->send( pack ) == sf::Socket::Done ) {
		Env::Reset( "RESULT" );
	} else {
		Env::SetVar( "RESULT", "FAIL" );
	}
	return OK;
}

AL_FUNC_FIX_ARG( recv, 2, false, false )
{
	std::string socket_loc;
	EVAL_AND_CHECK( "recv", args[ 0 ], socket_loc );
	socket_loc = Env::GetVar( socket_loc );
	CHECK_VAR_NUMERIC( args[ 0 ], socket_loc );
	sf::TcpSocket * sock = ( sf::TcpSocket * )std::stoull( socket_loc );
	if( sock == nullptr ) {
		std::cerr << "libnet: Failed to fetch TcpSocket's location from: " << socket_loc << "\n";
		return FAIL;
	}

	std::string data;
	EVAL_AND_CHECK( "recv", args[ 1 ], data );

	sf::Packet pack;
	if( sock->receive( pack ) == sf::Socket::Done ) {
		std::string recv_data;
		pack >> recv_data;
		Env::SetVar( data, recv_data );
	} else {
		Env::Reset( data );
	}
	return OK;
}