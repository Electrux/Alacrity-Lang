/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <array>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <future>

#include "../include/Errors.hpp"
#include "../include/String.hpp"
#include "../include/IO.hpp"
#include "../include/FS.hpp"
#include "../include/Env.hpp"

std::atomic< int > Env::threadctr( 0 );

std::string Env::GetVar( const std::string & var )
{
	char * val = getenv( var.c_str() );
	if( val == nullptr ) return "";
	return val;
}

bool Env::SetVar( const std::string & var, const std::string & val )
{
	return setenv( var.c_str(), val.c_str(), 1 ) == 0;
}

std::string Env::Shell()
{
	static std::string shell = GetVar( "SHELL" );
	if( shell.empty() ) return "/bin/sh";
	return shell;
}

std::string Env::Home()
{
	static std::string home = GetVar( "HOME" );
	if( home.empty() ) return "";
	return home;
}

std::vector< std::unordered_map< std::string, std::string > > & Env::Backups()
{
	static std::vector< std::unordered_map< std::string, std::string > > env_back = {};
	return env_back;
}

void Env::Backup()
{
	std::string env;
	Exec( "env", & env );
	Backups().push_back( Str::GenEnvMap( env ) );
}

bool Env::Restore()
{
	if( Backups().empty() ) return false;
	auto map = * ( Backups().end() - 1 );
	for( auto & m : map ) {
		if( !SetVar( m.first, m.second ) ) { Backups().pop_back(); return false; }
	}
	Backups().pop_back();
	return true;
}

int Env::ExecVector( const std::string & cmd, std::vector< std::string > * vec )
{
	std::string fcmd = cmd;
	if( vec == nullptr ) fcmd += " 1>/dev/null";
	FILE * pipe = popen( fcmd.c_str(), "r" );
	if( !pipe ) return POPEN_FAILED;

	if( vec != nullptr ) {
		char * line = NULL;
		size_t len = 0;
		ssize_t nread;

		vec->clear();
		while( ( nread = getline( & line, & len, pipe ) ) != -1 ) {
			vec->push_back( line );
		}
		free( line );
	}
	int res = pclose( pipe );
	res = WEXITSTATUS( res );
	SetVar( "EXIT_STATUS", std::to_string( res ) );
	return res;
}

int Env::Exec( const std::string & cmd, std::string * out )
{
	int res;
	if( out != nullptr ) {
		out->clear();
		std::vector< std::string > tmp;
		res = ExecVector( cmd, & tmp );
		for( auto & s : tmp ) {
			* out += s;
		}
	} else {
		res = ExecVector( cmd );
	}
	return res;
}

std::string Env::WrapShell( const std::string & cmd )
{
	return Shell() + " -c \"" + cmd + "\"";
}

std::string Env::GetDirPart( const std::string & full_loc )
{
	auto loc = full_loc.find_last_of( '/' );
	if( loc == std::string::npos ) return ".";
	if( loc == 0 ) return "/";
	return full_loc.substr( 0, loc );
}

std::string Env::GetFilePart( const std::string & full_loc )
{
	auto loc = full_loc.find_last_of( '/' );
	if( loc == std::string::npos ) return full_loc;
	if( loc == 0 ) return full_loc.substr( 1 );
	return full_loc.substr( loc + 1 );
}

std::string Env::GetFileLocation( const std::string & paths, const std::string & file, const char delim )
{
	std::string tmp;
	for( auto it = paths.begin(); it != paths.end(); ++it ) {
		auto p = * it;
		if( p == delim || it == paths.end() - 1 ) {
			if( it == paths.end() - 1 ) tmp += * it;
			if( !tmp.empty() ) {
				if( * ( tmp.end() - 1 ) != '/' ) tmp += "/";
				tmp += file;
				if( FS::LocExists( tmp ) ) return tmp;
				tmp.clear();
			}
			continue;
		}
		tmp += p;
	}
	return "";
}

void Env::Append( const std::string & var, const std::string & val, const char delim )
{
	if( val.empty() ) return;
	std::string env_val = GetVar( var );
	if( !env_val.empty() && !val.empty() ) {
		env_val += delim;
	}
	env_val += val;
	SetVar( var, env_val );
}

void Env::Prepend( const std::string & var, const std::string & val, const char delim )
{
	if( val.empty() ) return;
	std::string env_val = GetVar( var );
	if( !env_val.empty() && !val.empty() ) {
		env_val = val + delim + env_val;
	} else {
		env_val += val;
	}
	SetVar( var, env_val );
}

void Env::Reset( const std::string & var )
{
	std::string env_val = GetVar( var );
	if( !env_val.empty() ) SetVar( var, "" );
}

void Env::Remove( const std::string & var, const std::string & val, const char delim )
{
	std::string env_val = GetVar( var );
	auto loc = env_val.find( val );
	if( loc == std::string::npos ) return;
	if( env_val.size() == val.size() ) env_val.clear();
	std::string p1( env_val.substr( 0, loc ) );
	std::string p2( env_val.substr( loc + val.size() + 1 ) );
	if( ( p1.size() > 0 && p1[ p1.size() - 1 ] == delim ) || ( p2.size() > 0 && p2[ 0 ] == delim ) ) {
		if( p1.size() > 0 ) p1.erase( p1.end() - 1 );
		else if( p2.size() > 0 ) p2.erase( p2.begin() );
	}
	Env::SetVar( var, p1 + p2 );
}

int Env::MultiThreadedExec( const std::vector< ExecData > cmds )
{
	if( cmds.size() == 0 ) return OK;
	std::vector< std::future< int > > futures;
	int cores = std::thread::hardware_concurrency();
	int ctr = 1;
	int percent;
	for( auto cmdit = cmds.begin(); cmdit < cmds.end() - 1; ++cmdit ) {
		auto & cmd = * cmdit;
		percent = ( ctr * 100 ) / cmds.size();
		while( threadctr >= cores ) {
			for( auto it = futures.begin(); it != futures.end(); ) {
				if( it->wait_for( std::chrono::seconds( 0 ) ) != std::future_status::ready ) {
					++it;
					continue;
				}
				--threadctr;
				int res = it->get();
				it = futures.erase( it );
				if( res != 0 ) {
					for( auto & d : futures ) d.wait();
					return res;
				}
				break;
			}
		}
		IO::colout( false ) << "[" << percent << "%]\t" << cmd.msg << "\n";
		++ctr;
		++threadctr;
		futures.push_back( std::async( std::launch::async, ExecVector, cmd.cmd, nullptr ) );
	}

	for( auto it = futures.begin(); it != futures.end(); ) {
		int res = it->get();
		--threadctr;
		it = futures.erase( it );
		if( res != 0 ) {
			for( auto & d : futures ) d.wait();
			return res;
		}
	}

	IO::colout( false ) << "[" << 100 << "%]\t" << cmds[ cmds.size() - 1 ].msg << "\n";
	return Env::ExecVector( cmds[ cmds.size() - 1 ].cmd, nullptr );
}