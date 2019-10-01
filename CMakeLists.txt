cmake_minimum_required(VERSION 2.6)

project(orlib2ki)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)

if(CMAKE_TOOLCHAIN_FILE)
	set(LIBXML2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/win/include")
	set(LIBXML2_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/win/lib/libxml2.dll.a")
else()
	find_package(LibXml2 REQUIRED)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${LIBXML2_INCLUDE_DIR})

add_executable(orlib2ki ${SOURCES})

target_link_libraries(orlib2ki ${LIBXML2_LIBRARIES} m)
