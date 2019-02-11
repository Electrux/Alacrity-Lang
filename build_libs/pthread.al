if( "${CC}" == "" ) {
	fail( "CC environment variable is not set!\n" )
}

if( "${C_CPP_PTHREAD_SUPPORT}" == "" ) {
	load_file( "tests/c_cpp_pthread_support" )
}

if( "${C_CPP_PTHREAD_SUPPORT}" == no ) {
	if( "${OPTIONAL_VAR}" == "" ) {
		fail( "The pthread library is not supported by your compiler!\n" )
	}
	if( "${OPTIONAL_VAR}" != 0 ) {
		builds.add_c_flags( "${OPTIONAL_VAR}" )
		builds.add_cxx_flags( "${OPTIONAL_VAR}" )
	}
}

if( "${OS}" == OS_OSX || "${OS}" == OS_LINUX || "${OS}" == OS_BSD ) {
	builds.add_c_lib_flags( "-lpthread" )
	builds.add_cxx_lib_flags( "-lpthread" )
}
