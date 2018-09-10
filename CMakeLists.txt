
cmake_minimum_required(VERSION 3.10)
project(MAGMA)

set(MAGMA_MAJOR 0)
set(MAGMA_MINOR 1)
set(MAGMA_MICRO 0)


SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/lib CACHE
    PATH "Directory where all the .lib files are dumped." FORCE)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin CACHE
    PATH "Directory where .exe and .dll files are dumped." FORCE)

option(MAGMA_SHARED "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build Static libraries" OFF)

add_subdirectory(Dependencies)
add_subdirectory(Source)