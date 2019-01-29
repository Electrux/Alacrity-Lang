list.create( l )

list.push( l, sup )
list.push( l, yo )
list.push( l, something )
list.push( l, hi )

list.at( l, 2 )

print( "l[ 2 ] = ${RESULT}\n" )

print( "List elements are: " )
list.each( l, x ) {
	.print( "${x} " )
}
print( "\n" )

print( "Elements except 'something' are: " )
list.each( l, x ) {
	if( "${x}" == something ) {
		.contloop()
	}
	.print( "${x} " )
}
print( "\n" )

print( "Erasing l[ 1 ]\n" )
list.erase( l, 1 )

print( "After erasing, list elements are: " )
list.each( l, x ) {
	.print( "${x} " )
}
print( "\n" )

print( "Popping l\n" )
list.pop( l )

print( "After popping, list elements are: " )
list.each( l, x ) {
	.print( "${x} " )
}
print( "\n" )

print( "Clearing list\n" )
list.clear( l )

print( "After clearing, list elements are: " )
list.each( l, x ) {
	.print( "${x} " )
}
print( "\n" )

list.destroy( l )
