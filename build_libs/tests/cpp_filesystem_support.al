if( "${CPP_FILESYSTEM_SUPPORT}" == "" ) {
	print( "Checking filesystem support... " )
	if( "${OS}" == OS_OSX ) {
		eval( "#{${CC} -std=c++17 cpp_filesystem_support.cpp -I/usr/local/opt/llvm/include -L/usr/local/opt/llvm/lib -lc++fs}" )
	} elif( "${OS}" == OS_LINUX ) {
		eval( "#{${CC} -std=c++17 cpp_filesystem_support.cpp -lstdc++fs}" )
	}
	if( "${EXIT_STATUS}" == 0 ) {
		exec( "rm a.out" )
		CPP_FILESYSTEM_SUPPORT = yes
		print( "{g}yes{0}\n" )
	} else {
		CPP_FILESYSTEM_SUPPORT = no
		print( "{r}no{0}\n" )
	}
}