project( "Alacrity-Lang" ) {
	version( 0.1 )
	language( cpp, 17 )
	license( bsd3 )
	author( Electrux, "ElectruxRedsworth@gmail.com" )
}

add_cxx_flags( "-O2", "-fPIC" )

al_add_src_paths( build_libs )

load_file( filesystem )
load_file( dl )

builds( bin ) {
	sources( "src/(.*)\.cpp", "-src/Modules/(.*)\.cpp" )
	build( al, "src/main.cpp" )
}

builds( lib, dynamic ) {
	sources( "src/(.*)\.cpp", "-src/main.cpp", "-src/Modules/(.*)\.cpp" )
	build( core, "src/Modules/core.cpp" )
	build( os, "src/Modules/os.cpp" )
	build( math, "src/Modules/math.cpp" )
	build( list, "src/Modules/list.cpp" )
}

builds( lib, dynamic ) {
	add_lib_flags( "-lsfml-audio" )
	build( audio, "src/Modules/audio.cpp" )
}
