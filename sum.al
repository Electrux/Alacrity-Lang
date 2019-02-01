set( "res", 0 )

for( x, 1, 10 ) {
	math.add( res, "${res}", "${x}" )
}

print( "Result: ${res}\n" )
