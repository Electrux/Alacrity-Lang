input_num( num, "Enter a number: " )
isprime = true
math.isprime( isprime, "${num}" )

if( "${isprime}" == true ) { print( "${num} is prime\n" ) }
else { print( "${num} is not prime\n" ) }