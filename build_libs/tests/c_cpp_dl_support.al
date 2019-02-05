if( "${C_CPP_DL_SUPPORT}" == "" ) {
	print( "Checking libdl support... " )

	if( "${LANGUAGE}" == "c++" ) {
		tmp_cc = "${CC}"
		if( "${CC}" == "g++" ) {
			CC = "gcc"
		} elif( "${CC}" == "clang++" ) {
			CC = "clang"
		}
	}

	eval( "#{${CC} c_cpp_dl_support.c -ldl}" )

	if( "${LANGUAGE}" == "c++" ) {
		CC = "${tmp_cc}"
		reset( tmp_cc )
	}

	if( "${EXIT_STATUS}" == 0 ) {
		exec( "rm a.out" )
		C_CPP_DL_SUPPORT = yes
		print( "{g}yes{0}\n" )
	} else {
		C_CPP_DL_SUPPORT = no
		print( "{r}no{0}\n" )
	}
}