#!/usr/bin/env bash

compiler="clang++"
os=$(uname)

if [[ "$os" == 'Linux' ]]; then
	compiler="g++"
fi

if ! [[ -z "${COMPILER}" ]]; then
	compiler="${COMPILER}"
fi

compiler_version=$($compiler --version)
echo "Using compiler: $compiler, version: $compiler_version"

rm -rf buildfiles 2>/dev/null

echo "Creating directories ..."

mkdir -p "buildfiles/src/Interpreter"
mkdir -p "buildfiles/src/Lexer"
mkdir -p "buildfiles/src/Modules/builds"
mkdir -p "buildfiles/src/Parser"

if [[ -z "$PREFIX" ]]; then
	PREFIX="${PWD}"
fi

# Binary: al

find src -name "*.cpp" | grep -v "Modules" | grep -v "main.cpp" | while read -r src_file; do
	echo "Compiling: $src_file ..."
	$compiler -O2 -fPIC -std=c++17 -c $src_file -o buildfiles/$src_file.o -I/usr/local/include
	if [[ $? != 0 ]]; then
		break
	fi
done

if [[ $? != 0 ]]; then
	echo "Error in compiling sources, will not continue"
	exit $?
fi

buildfiles=$(find buildfiles -name "*.cpp.o" | paste -sd " " -)

echo "Building binary: al ..."
$compiler -O2 -fPIC -std=c++17 -g -o buildfiles/al src/main.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread -DBUILD_PREFIX_DIR=${PREFIX}
if [[ $? != 0 ]]; then
	exit $?
fi

# Library: core
echo "Building libray: core ..."
$compiler -O2 -fPIC -std=c++17 -shared -o buildfiles/libcore.so src/Modules/core.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread -DBUILD_PREFIX_DIR=${PREFIX}
if [[ $? != 0 ]]; then
	exit $?
fi

# Library: time
echo "Building libray: time ..."
$compiler -O2 -fPIC -std=c++17 -shared -o buildfiles/libtime.so src/Modules/time.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread -DBUILD_PREFIX_DIR=${PREFIX}
if [[ $? != 0 ]]; then
	exit $?
fi

# Library: os
echo "Building libray: os ..."
$compiler -O2 -fPIC -std=c++17 -shared -o buildfiles/libos.so src/Modules/os.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread -DBUILD_PREFIX_DIR=${PREFIX}
if [[ $? != 0 ]]; then
	exit $?
fi

# Library: string
echo "Building libray: string ..."
$compiler -O2 -fPIC -std=c++17 -shared -o buildfiles/libstring.so src/Modules/string.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread -DBUILD_PREFIX_DIR=${PREFIX}
if [[ $? != 0 ]]; then
	exit $?
fi

# Library: math
echo "Building libray: math ..."
$compiler -O2 -fPIC -std=c++17 -shared -o buildfiles/libmath.so src/Modules/math.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread -DBUILD_PREFIX_DIR=${PREFIX}
if [[ $? != 0 ]]; then
	exit $?
fi

# Library: list
echo "Building libray: list ..."
$compiler -O2 -fPIC -std=c++17 -shared -o buildfiles/liblist.so src/Modules/list.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread -DBUILD_PREFIX_DIR=${PREFIX}
if [[ $? != 0 ]]; then
	exit $?
fi

# Library: audio
echo "Building libray: audio ..."
$compiler -O2 -fPIC -std=c++17 -shared -o buildfiles/libaudio.so src/Modules/audio.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread -lsfml-audio -DBUILD_PREFIX_DIR=${PREFIX}
#if [[ $? != 0 ]]; then
#	exit $?
#fi

# Library: network
echo "Building libray: network ..."
$compiler -O2 -fPIC -std=c++17 -shared -o buildfiles/libnet.so src/Modules/net.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread -lsfml-system -lsfml-network -DBUILD_PREFIX_DIR=${PREFIX}
#if [[ $? != 0 ]]; then
#	exit $?
#fi

# Library: project
echo "Building libray: project ..."
$compiler -O2 -fPIC -std=c++17 -shared -o buildfiles/libproject.so src/Modules/project.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread -DBUILD_PREFIX_DIR=${PREFIX}
if [[ $? != 0 ]]; then
	exit $?
fi

# Library: builds
echo "Building libray: builds ..."
$compiler -O2 -fPIC -std=c++17 -shared -o buildfiles/libbuilds.so src/Modules/builds.cpp src/Modules/builds/c_cxx.cpp $buildfiles -I/usr/local/include -L/usr/local/lib -ldl -lpthread
if [[ $? != 0 ]]; then
	exit $?
fi

# Install this

mkdir -p "$PREFIX/share/allang_tests/"

if [[ $? != 0 ]]; then
	echo "You might wanna run this as root for installation!"
	exit $?
fi

mkdir -p "$PREFIX/share/allang_libs/"

cp_cmd="cp -r "

if [[ "$os" == 'Linux' ]]; then
	cp_cmd="cp -r --remove-destination "
fi

echo "Installing files ..."
$cp_cmd buildfiles/al "$PREFIX/bin"
$cp_cmd buildfiles/lib*.so "$PREFIX/share/allang_libs/"
$cp_cmd build_libs/* "$PREFIX/share/allang_tests/"
