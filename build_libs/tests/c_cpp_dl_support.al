print( "{b}Checking for libdl support{0} ... " )

if( "${LANGUAGE}" == "c++" ) {
	tmp_cc = "${CC}"
	if( "${CC}" == "g++" ) {
		CC = "gcc"
	} elif( "${CC}" == "clang++" ) {
		CC = "clang"
	}
}

eval( "#{${CC} c_cpp_dl_support.c -ldl -o /dev/null 2>/dev/null}" )

if( "${LANGUAGE}" == "c++" ) {
	CC = "${tmp_cc}"
	reset( tmp_cc )
}

if( "${EXIT_STATUS}" == 0 ) {
	C_CPP_DL_SUPPORT = yes
	print( "{g}yes{0}\n" )
} else {
	C_CPP_DL_SUPPORT = no
	print( "{r}no{0}\n" )
}