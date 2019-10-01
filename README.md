# orlib2ki
Orcad to KiCAD library

# Build for Windows (mingw-w64)

	sudo apt-get install mingw-w64

	mkdir build && cd build
	cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-mingw32.cmake ..

# Build for Linux

	sudo apt-get install libxml2-dev
	mkdir build && cd build
	cmake ..
