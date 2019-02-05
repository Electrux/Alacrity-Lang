#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <iostream>
#include <vector>
#include <string>

#include "../../include/Errors.hpp"
#include "../../include/String.hpp"
#include "../../include/Env.hpp"
#include "../../include/FS.hpp"

class Builder
{
	std::string m_build_type;
	std::string m_lib_type;

	std::vector< std::string > m_srcs;
	std::string m_main_src;

	Builder();
public:
	static Builder * Get();

	void Reset();

	std::vector< std::string > & Sources();
	std::string & MainSource();

	int SetBuildType( const std::string & build_type );
	const std::string & GetBuildType();

	int SetLibType( const std::string & lib_type );
	const std::string & GetLibType();

	int Build();
};

Builder::Builder() {}

Builder * Builder::Get()
{
	static Builder builder;
	return & builder;
}

void Builder::Reset()
{
	m_build_type = "bin";
	m_lib_type = "";
	m_srcs.clear();
	m_main_src.clear();
}

std::vector< std::string > & Builder::Sources() { return m_srcs; }
std::string & Builder::MainSource() { return m_main_src; }

int Builder::SetBuildType( const std::string & build_type )
{
	int res = OK;
	std::string err;
	if( build_type.empty() ) {
		m_build_type = "bin";
		return OK;
	}

	if( build_type != "bin" && build_type != "lib" && build_type != "test" ) {
		err = "Invalid build type: " + build_type;
		res = BUILDS_INVALID_TYPE;
		goto error;
	}

	m_build_type = build_type;
	return res;
error:
	std::string op;
	if( Str::Eval( err + "{0}", op ) != OK ) {
		err = "Failed to evaluate error: " + err;
	}
	std::cout << "{r}SetBuildType() Error{0}: " << op << "!\n";
	return res;
}

const std::string & Builder::GetBuildType() { return m_build_type; }

int Builder::SetLibType( const std::string & lib_type )
{
	int res = OK;
	std::string err;
	if( m_build_type != "lib" ) {
		err = "Attempting to set library type when the build type is: " + m_build_type;
		res = BUILDS_LIB_TYPE_WHEN_NON_LIB;
		goto error;
	}

	if( lib_type.empty() ) {
		m_lib_type = "dynamic";
		return OK;
	}

	if( lib_type != "static" && lib_type != "dynamic" ) {
		err = "Invalid library type: " + lib_type;
		res = BUILDS_INVALID_TYPE;
		goto error;
	}

	m_lib_type = lib_type;
	return res;
error:
	std::string op;
	if( Str::Eval( err + "{0}", op ) != OK ) {
		err = "Failed to evaluate error: " + err;
	}
	std::cout << "{r}SetBuildType() Error{0}: " << op << "!\n";
	return res;
}

const std::string & Builder::GetLibType() { return m_lib_type; }

#endif // BUILDER_HPP