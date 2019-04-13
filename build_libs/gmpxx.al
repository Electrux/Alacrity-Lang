if( "${CC}" == "" ) {
	fail( "CC environment variable is not set!\n" )
}

if( "${LANGUAGE}" != "c++" ) {
	fail( "Only C++ supports filesystems\n" )
}

if( "${CPP_GMPXX_SUPPORT}" == "" ) {
	load_file( "tests/cpp_gmpxx_support" )
}

if( "${CPP_GMPXX_SUPPORT}" == no ) {
	if( "${OPTIONAL_VAR}" == "" ) {
		fail( "The gmp c++ library is not supported by your compiler!\n" )
	}
	if( "${OPTIONAL_VAR}" != 0 ) {
		builds.add_flags( "${OPTIONAL_VAR}" )
	}
} else {
	builds.add_lib_flags( "-lgmpxx -lgmp" )
}
