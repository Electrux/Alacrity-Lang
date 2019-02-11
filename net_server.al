net.local_ip( local_ip )
print( "Local IP: ${local_ip}\n" )

net.public_ip( public_ip )
print( "Public IP: ${public_ip}\n" )

list.create( socks )
list.create( socks_rem )
net.listen( svr, 8080 )
print( "Listening on port: 8080 ...\n" )

for() {
	net.accept( svr, sock )
	if( "${sock}" != "" ) {
		list.push( socks, "${sock}" )
		net.sock_ip( sock, ip )
		.print( "Added client: ${ip}\n" )
	}

/*
	net.each_recv( socks, sock, data ) {
		net.sock_ip( sock, ip )
		.print( "Received from ${ip}: ${data}\n" )
	}
*/

	list.each( sock, socks ) {
		net.recv( sock, data )
		if( "${data}" != "" ) {
			net.sock_ip( sock, ip )
			.print( "Received from ${ip}: ${data}\n" )
		}

		net.disconnected( sock, dc )
		if( "${dc}" == true ) {
			.print( "A client disconnected!\n" )
			list.push( socks_rem, "${sock}" )
			.contloop()
		}
	}
	list.each( sock, socks_rem ) {
		.print( "Removing socket ${sock}\n" )
		list.erase( socks, "${sock}" )
	}
	list.clear( socks_rem )
}

net.unlisten( svr )
list.destroy( socks_rem )
list.destroy( socks )