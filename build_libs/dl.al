if( "${CC}" == "" ) {
	fail( "CC environment variable is not set!\n" )
}

if( "${C_CPP_DL_SUPPORT}" == "" ) {
	load_file( "tests/c_cpp_dl_support" )
}

if( "${C_CPP_DL_SUPPORT}" == no ) {
	if( "${OPTIONAL_VAR}" == "" ) {
		fail( "The dl library is not supported by your compiler!\n" )
	}
	if( "${OPTIONAL_VAR}" != 0 ) {
		builds.add_c_flags( "${OPTIONAL_VAR}" )
		builds.add_cxx_flags( "${OPTIONAL_VAR}" )
	}
}

if( "${OS}" == OS_OSX || "${OS}" == OS_LINUX || "${OS}" == OS_BSD ) {
	builds.add_c_lib_flags( "-ldl" )
	builds.add_cxx_lib_flags( "-ldl" )
}
