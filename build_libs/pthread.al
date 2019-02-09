if( "${CC}" == "" ) {
	fail( "CC environment variable is not set!\n" )
}

load_file( "tests/c_cpp_pthread_support" )

if( "${C_CPP_PTHREAD_SUPPORT}" == no || "${C_CPP_PTHREAD_SUPPORT}" == "" ) {
	fail( "The pthread library is not supported by your compiler!\n" )
}

if( "${OS}" == OS_OSX || "${OS}" == OS_LINUX || "${OS}" == OS_BSD ) {
	builds.add_c_lib_flags( "-lpthread" )
	builds.add_cxx_lib_flags( "-lpthread" )
}
