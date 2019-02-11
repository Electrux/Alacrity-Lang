print( "{b}Checking yaml-cpp support{0} ... " )
if( "${OS}" == OS_OSX ) {
	eval( "#{${CC} -std=c++11 cpp_yaml_cpp_support.cpp -o /tmp/a.out -lyaml-cpp 2>/dev/null}" )
} elif( "${OS}" == OS_LINUX ) {
	eval( "#{${CC} -std=c++11 cpp_yaml_cpp_support.cpp -o /tmp/a.out -lyaml-cpp 2>/dev/null}" )
} elif( "${OS}" == OS_BSD ) {
	eval( "#{${CC} -std=c++11 cpp_yaml_cpp_support.cpp -o /tmp/a.out -I/usr/local/include -L/usr/local/lib -lyaml-cpp 2>/dev/null}" )
}
if( "${EXIT_STATUS}" == 0 ) {
	exec( "rm /tmp/a.out" )
	CPP_YAML_CPP_SUPPORT = yes
	print( "{g}yes{0}\n" )
} else {
	CPP_YAML_CPP_SUPPORT = no
	print( "{r}no{0}\n" )
}