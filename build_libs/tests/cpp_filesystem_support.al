print( "{b}Checking for filesystem support{0} ... " )
if( "${OS}" == OS_OSX ) {
	eval( "#{${CC} -std=c++17 cpp_filesystem_support.cpp -o /dev/null -I/usr/local/opt/llvm/include -L/usr/local/opt/llvm/lib -lc++fs 2>/dev/null}" )
} elif( "${OS}" == OS_LINUX ) {
	eval( "#{${CC} -std=c++17 cpp_filesystem_support.cpp -o /dev/null -lstdc++fs 2>/dev/null}" )
} elif( "${OS}" == OS_BSD ) {
	eval( "#{${CC} -std=c++17 cpp_filesystem_support.cpp -o /dev/null -lc++experimental 2>/dev/null}" )
}
if( "${EXIT_STATUS}" == 0 ) {
	CPP_FILESYSTEM_SUPPORT = yes
	print( "{g}yes{0}\n" )
} else {
	CPP_FILESYSTEM_SUPPORT = no
	print( "{r}no{0}\n" )
}