if( "${CC}" == "" ) {
	fail( "CC environment variable is not set!\n" )
}

if( "${LANGUAGE}" != "c++" ) {
	fail( "Only C++ supports yaml-cpp\n" )
}

if( "${STD_VERSION}" < 11 ) {
	fail( "Minimum required standard version for yaml-cpp library is c++11\n" )
}

if( "${CPP_YAML_CPP_SUPPORT}" == "" ) {
	load_file( "tests/cpp_yaml_cpp_support" )
}

if( "${CPP_YAML_CPP_SUPPORT}" == no ) {
	if( "${OPTIONAL_VAR}" == "" ) {
		fail( "The yaml-cpp library is not supported by your compiler!\n" )
	}
	if( "${OPTIONAL_VAR}" != 0 ) {
		builds.add_c_flags( "${OPTIONAL_VAR}" )
		builds.add_cxx_flags( "${OPTIONAL_VAR}" )
	}
}

if( "${OS}" == OS_BSD ) {
	builds.add_cxx_inc_dirs( "-I/usr/local/include" )
	builds.add_cxx_lib_dirs( "-L/usr/local/lib" )
}
builds.add_cxx_lib_flags( "-lyaml-cpp" )