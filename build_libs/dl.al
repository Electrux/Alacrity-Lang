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
		builds.add_flags( "${OPTIONAL_VAR}" )
	}
} else {
	builds.add_lib_flags( "-ldl" )
}
