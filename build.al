project( "Alacrity-Lang" ) {
	version( 0.1 )
	language( cpp, 17 )
	license( bsd3 )
	author( Electrux, "ElectruxRedsworth@gmail.com" )
}

builds.add_cxx_flags( "-O2", "-fPIC" )

load_file( filesystem )
load_file( pthread )
load_file( dl )

builds( bin ) {
	sources( "./src/(.*)\.cpp", "-src/Modules/(.*)\.cpp" )
	build( al, "src/main.cpp" )
}

builds( lib, dynamic ) {
	sources( "src/(.*)\.cpp", "-src/main.cpp", "-src/Modules/(.*)\.cpp" )
	build( core, "src/Modules/core.cpp" )
	build( os, "src/Modules/os.cpp" )
	build( string, "src/Modules/string.cpp" )
	build( math, "src/Modules/math.cpp" )
	build( list, "src/Modules/list.cpp" )
	build( audio, "src/Modules/audio.cpp", "", "-lsfml-audio" )
	build( project, "src/Modules/project.cpp" )
	build( builds, "src/Modules/builds.cpp, src/Modules/builds/cxx.cpp, src/Modules/builds/c.cpp" )
}

if( "${ARGC}" > 0 && "${ARG_0}" == "install" ) {
	if( "${IS_ROOT}" == "true" || "${OS}" == OS_OSX ) {
		install( "buildfiles/al", "/usr/local/bin" )
		install( "buildfiles/lib*.so", "/usr/local/share/allang_libs/" )
		install( "build_libs/*", "/usr/local/share/allang_tests/" )
	}
}