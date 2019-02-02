audio.play( mnd, "wave.wav" )
vol = 100
is_inc = true
for( x, 1, 2000 ) {
	if( "${vol}" >= 100 ) {
		is_inc = false
	}
	if( "${vol}" <= 10 ) {
		is_inc = true
	}
	if( "${is_inc}" == true ) {
		math.inc( vol )
	} else {
		math.dec( vol )
	}
	audio.setvol( mnd, "${vol}" )
	os.sleep( 10 )
}

audio.stop( mnd )