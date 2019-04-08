str = "Hello world"

string.each_idx( i, x, "${str}" ) {
	if( "${x}" != " " ) { add( str, "${i}", 1 ) }
}
print( "${str}\n" )