# all the variables will be stored as Env Vars
# and all variables are in caps

# can use '.' at beginning/end of function name to mark it global
# example: inside project, if version was named 'version.',
# it will make version a single whole function instead of
# project/version

# can use project.{version,language,...} if one doesn't
# wants to use the {} syntax

# also, if a function's library file isn't found,
# it will fallback to searching it in the core library
# (libcore.so), after which, should it fail, the system
# will be taken down

project( EBS ) {
	version( 0.0.1 )
	language( cpp, 17 )
	license( bsd3 )
	author( Electrux, "ElectruxRedsworth@gmail.com" )
}

# the variables like paths are visited in order,
# adding a value using prepend will give it highest priority
# we can also use bs_add_search_path( "libfiles" ) instead of
# the raw call below
bs_add_search_paths( "lib_files" )

# use_lib, add_cxx_flags can be specific to a particular build as well
# they will be backed up in any function with a block, and reverted
# after the block 
disable_internal_display()
use_lib( filesystem )
use_lib( dl )
enable_internal_display()

add_cxx_flags( "-O2", "-fPIC" )

if( "${LOOP_TEST}" != "" ) {
	use_lib( looptest )
}

# build types are:
#   binary
#   library
#   test
builds( binary ) {
	sources( "src/(.*)\.cpp", "-src/Interpreter/Functions/(.*)\.cpp" )
	build( ebs, "src/main.cpp" )
}

builds( library, dynamic ) {
	sources( "src/(.*)\.cpp", "-src/main.cpp" )
	build( core, "src/Interpreter/Functions/core.cpp", "-src/Interpreter/Functions/.([^\/]*)\.cpp" )
	build( project, "src/Interpreter/Functions/project.cpp", "-src/Interpreter/Functions/(.*)\.cpp" )
	build( builds, "src/Interpreter/Functions/builds.cpp", "-src/Interpreter/Functions/(.*)\.cpp" )
}

builds( test ) {

}
