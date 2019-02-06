if( "${C_CPP_PTHREAD_SUPPORT}" == "" ) {
	print( "{b}Checking pthread support{0} ... " )

	if( "${LANGUAGE}" == "c++" ) {
		tmp_cc = "${CC}"
		if( "${CC}" == "g++" ) {
			CC = "gcc"
		} elif( "${CC}" == "clang++" ) {
			CC = "clang"
		}
	}

	eval( "#{${CC} c_cpp_pthread_support.c -o /tmp/a.out -lpthread 2>/dev/null}" )

	if( "${LANGUAGE}" == "c++" ) {
		CC = "${tmp_cc}"
		reset( tmp_cc )
	}

	if( "${EXIT_STATUS}" == 0 ) {
		exec( "rm /tmp/a.out" )
		C_CPP_PTHREAD_SUPPORT = yes
		print( "{g}yes{0}\n" )
	} else {
		C_CPP_PTHREAD_SUPPORT = no
		print( "{r}no{0}\n" )
	}
}