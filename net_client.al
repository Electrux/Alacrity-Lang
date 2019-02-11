print( "Connecting to server ...\n" )
net.connect( sock, localhost, 8080 )

if( "${RESULT}" != "" ) {
	fail( "Failed to connect to server at socket: 8080\n" )
}

print( "Sending message ...\n" )
net.send( sock, "Hello, server" )
if( "${RESULT}" != "" ) {
	net.disconnect( sock )
	fail( "Failed to send data to server\n" )
}

print( "Disconnecting ...\n" )
net.disconnect( sock )