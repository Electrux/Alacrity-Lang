if( "${CC}" == "" ) {
	fail( "CC environment variable is not set!\n" )
}

if( "${LANGUAGE}" != "c++" ) {
	fail( "Only C++ supports SFML\n" )
}

if( "${STD_VERSION}" < 11 ) {
	fail( "Minimum required standard version for SFML audio library is c++11\n" )
}

if( "${CPP_SFML_AUDIO_SUPPORT}" == "" ) {
	load_file( "tests/cpp_sfml_audio_support" )
}

if( "${CPP_SFML_AUDIO_SUPPORT}" == no ) {
	if( "${OPTIONAL_VAR}" == "" ) {
		fail( "The sfml audio library is not supported by your compiler!\n" )
	}
	if( "${OPTIONAL_VAR}" != 0 ) {
		builds.add_flags( "${OPTIONAL_VAR}" )
	}
}

if( "${OS}" == OS_BSD ) {
	builds.add_inc_dirs( "-I/usr/local/include" )
	builds.add_lib_dirs( "-L/usr/local/lib" )
}
builds.add_lib_flags( "-lsfml-audio" )