input_num( num, "Enter a number: " )
set( isprime, true )
set( sqrnum, "%{echo \"sqrt(${num}) + 1\" | bc}" )

foreach( x, 2, "${sqrnum}" ) {
	set( tmp, "%{echo ${num} % ${x} | bc}" )
	if( "${tmp}" == 0 ) {
		set( isprime, false )
		brkloop()
	}
}

if( "${isprime}" == true ) {
	print( "${num} is prime\n" )
} else {
	print( "${num} is not prime\n" )
}