print( "{b}Checking for libgmpxx support{0} ... " )

eval( "#{${CC} cpp_gmpxx_support.cpp -lgmpxx -lgmp -o /dev/null 2>/dev/null}" )

if( "${EXIT_STATUS}" == 0 ) {
	CPP_GMPXX_SUPPORT = yes
	print( "{g}yes{0}\n" )
} else {
	CPP_GMPXX_SUPPORT = no
	print( "{r}no{0}\n" )
}
