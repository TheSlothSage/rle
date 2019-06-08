cmake_minimum_required(VERSION 3.13.1)
cmake_minimum_required(VERSION 3.13.1 FATAL_ERROR)

project(rle)
enable_language(CXX)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(STATIC_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/libs) 

set (RLE_MAJOR 1)
set (RLE_MINOR 0)
set (RLE_PATCH 0)

file(GLOB_RECURSE HEADERS FOLLOW_SYMLINKS "include/*.h*")
file(GLOB_RECURSE RLE_SOURCES FOLLOW_SYMLINKS "src/*.c*")
file(GLOB_RECURSE RLE_LIBS FOLLOW_SYMLINKS "libs/*.a")

file (GLOB_RECURSE LUA_SRC FOLLOW_SYMLINKS "lua53/src/*.c*")
file (GLOB_RECURSE LUA_INC FOLLOW_SYMLINKS "lua53/include/*.h*")

set(HEADER_DIRS "")
foreach(header ${HEADERS})
	get_filename_component(_dir ${header} DIRECTORY)
	list (APPEND HEADER_DIRS ${_dir})
endforeach()



list(REMOVE_DUPLICATES HEADER_DIRS)
include_directories(${HEADER_DIRS})
include_directories(/home/projects/LuaBridge/Souce/LuaBridge) 
link_directories(${STATIC_LIBRARY_PATH}) 

add_subdirectory(glfw)
add_library(liblua ${LUA_SRC})

add_library(rle ${RLE_SOURCES})  
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()
set_property(TARGET rle PROPERTY CXX_STANDARD 17)

target_link_libraries(rle liblua)

add_subdirectory(test)
