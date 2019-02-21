cmake_minimum_required(VERSION 3.13.1)
cmake_minimum_required(VERSION 3.13.1 FATAL_ERROR)

project(rle_testing)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(STATIC_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/libs) 

set (RLE_TEST_MAJOR 1)
set (RLE_TEST_MINOR 0)
set (RLE_TEST_PATCH 0)

file(GLOB_RECURSE HEADERS FOLLOW_SYMLINKS "include/*.h*")
file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS "src/*.c*")
file(GLOB_RECURSE PROJECT_LIBS FOLLOW_SYMLINKS "libs/*.a")

set(HEADER_DIRS "")
foreach(header ${HEADERS})
	get_filename_component(_dir ${header} DIRECTORY)
	list (APPEND HEADER_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES HEADER_DIRS)
include_directories(${HEADER_DIRS}) 

link_directories(${STATIC_LIBRARY_PATH}) 

add_executable(rle_test ${PROJECT_SOURCES})
target_link_libraries(rle_test rle)
  
