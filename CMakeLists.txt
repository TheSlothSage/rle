cmake_minimum_required(VERSION 3.5.1)
project(rle)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(STATIC_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/libs) 
set(LUA_HEADERS_PATH ${CMAKE_SOURCE_DIR}/lua/src) 

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set (RLE_MAJOR 1)
set (RLE_MINOR 0)
set (RLE_PATCH 0)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS}})

file(GLOB_RECURSE HEADERS FOLLOW_SYMLINKS "include/*.h*")
file(GLOB_RECURSE RLE_SOURCES FOLLOW_SYMLINKS "src/*.c*")
file(GLOB_RECURSE RLE_LIBS FOLLOW_SYMLINKS "libs/*.a")

file(GLOB_RECURSE LUA_HEADERS FOLLOW_SYMLINKS "lua/src/lua*.h*")

set(HEADER_DIRS "include")
foreach(header ${HEADERS})
	get_filename_component(_dir ${header} DIRECTORY)
	list (APPEND HEADER_DIRS ${_dir})
endforeach()

list(REMOVE_DUPLICATES HEADER_DIRS)
include_directories(${HEADER_DIRS} ${LUA_HEADERS_PATH})
link_directories(${STATIC_LIBRARY_PATH}) 



add_library(rle ${RLE_SOURCES}) 

target_link_libraries(rle lua dl ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

add_subdirectory(test)
