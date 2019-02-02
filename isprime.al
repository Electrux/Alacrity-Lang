input_num( num, "Enter a number: " )
isprime = true
sqrnum = "%{echo \"sqrt(${num}) + 1\" | bc}"

foreach( x, 2, "${sqrnum}" ) {
	tmp = "%{echo ${num} % ${x} | bc}"
	if( "${tmp}" == 0 ) {
		isprime = false
		brkloop()
	}
}

if( "${isprime}" == true ) {
	print( "${num} is prime\n" )
} else {
	print( "${num} is not prime\n" )
}