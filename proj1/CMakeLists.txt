cmake_minimum_required(VERSION 3.6)
project(proj1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/Makefile
        cmake-build-debug/proj1
        cmake-build-debug/proj1.cbp
        CMakeLists.txt
        hashtable.c
        hashtable.h
        Makefile
        philspel.c
        philspel.h
        sampleDictionary
        sampleInput
        sampleOutput
        testOutput)

add_executable(proj1 ${SOURCE_FILES})