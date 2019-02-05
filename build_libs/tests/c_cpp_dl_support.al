if( "${C_CPP_DL_SUPPORT}" == "" ) {
	#disable_internal_display()
	print( "Checking libdl support... " )

	if( "${LANGUAGE}" == cpp ) {
		set( tmp_cc, "${CC}" )
		if( "${CC}" == "g++" ) {
			set( CC, "gcc" )
		} elif( "${CC}" == "clang++" ) {
			set( CC, "clang" )
		}
	}

	eval( "#{${CC} c_cpp_dl_support.c -ldl}" )

	if( "${LANGUAGE}" == cpp ) {
		set( CC, "${tmp_cc}" )
		reset( tmp_cc )
	}

	if( "${EXIT_STATUS}" == 0 ) {
		exec( "rm a.out" )
		set( C_CPP_DL_SUPPORT, yes )
		print( "{g}yes{0}\n" )
	} else {
		set( C_CPP_DL_SUPPORT, no )
		print( "{r}no{0}\n" )
	}

	#enable_internal_display()
}