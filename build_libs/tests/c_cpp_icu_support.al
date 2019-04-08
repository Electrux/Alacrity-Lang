print( "{b}Checking for libicu support{0} ... " )

if( "${LANGUAGE}" == "c++" ) {
	tmp_cc = "${CC}"
	if( "${CC}" == "g++" ) {
		CC = "gcc"
	} elif( "${CC}" == "clang++" ) {
		CC = "clang"
	}
}

if( "${OS}" == OS_OSX ) {
	eval( "#{${CC} c_cpp_dl_support.c -I/usr/local/opt/icu4c/include -L/usr/local/opt/icu4c/lib -licuio -licui18n -licuuc -licudata -o /dev/null 2>/dev/null}" )
} else {
	eval( "#{${CC} c_cpp_dl_support.c -licuio -licui18n -licuuc -licudata -o /dev/null 2>/dev/null}" )
}

if( "${LANGUAGE}" == "c++" ) {
	CC = "${tmp_cc}"
	reset( tmp_cc )
}

if( "${EXIT_STATUS}" == 0 ) {
	C_CPP_ICU_SUPPORT = yes
	print( "{g}yes{0}\n" )
} else {
	C_CPP_ICU_SUPPORT = no
	print( "{r}no{0}\n" )
}