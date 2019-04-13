if( "${CC}" == "" ) {
	fail( "CC environment variable is not set!\n" )
}

if( "${C_CPP_ICU_SUPPORT}" == "" ) {
	load_file( "tests/c_cpp_icu_support" )
}

if( "${C_CPP_ICU_SUPPORT}" == no ) {
	if( "${OPTIONAL_VAR}" == "" ) {
		fail( "The icu library is not supported by your compiler!\n" )
	}
	if( "${OPTIONAL_VAR}" != 0 ) {
		builds.add_flags( "${OPTIONAL_VAR}" )
	}
} else {
	if( "${OS}" == OS_OSX ) {
		builds.add_inc_dirs( "-I/usr/local/opt/icu4c/include" )
		builds.add_lib_dirs( "-L/usr/local/opt/icu4c/lib" )
	}
	builds.add_lib_flags( "-licuio -licui18n -licuuc -licudata" )
}
