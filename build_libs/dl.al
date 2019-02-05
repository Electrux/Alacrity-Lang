if( "${CC}" == "" ) {
	fail( "CC environment variable is not set!\n" )
}

load_file( "tests/c_cpp_dl_support" )

if( "${C_CPP_DL_SUPPORT}" == no || "${C_CPP_DL_SUPPORT}" == "" ) {
	fail( "The dl library is not supported by your compiler!\n" )
}

if( "${OS}" == OS_OSX || "${OS}" == OS_LINUX ) {
	#add_c_lib_flags( "-ldl" )
	#add_cxx_lib_flags( "-ldl" )
}
