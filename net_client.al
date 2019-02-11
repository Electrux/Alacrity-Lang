print( "Connecting to server ...\n" )

net.connect( sock, localhost, 8080 )

if( "${RESULT}" != "" ) {
	fail( "Failed to connect to server at socket: 8080\n" )
}

net.send( sock, "Hello, server" )
if( "${RESULT}" != "" ) {
	fail( "Failed to send data to server\n" )
}

net.disconnect( sock )