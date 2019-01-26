set( "res", 0 )

foreach( x, 1, 10 ) {
	set( res, "%{echo ${res} + ${x} | bc}" )
}

print( "Result: ${res}\n" )
