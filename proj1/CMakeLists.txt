cmake_minimum_required(VERSION 3.6)
project(proj1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        hashtable.c
        hashtable.h
        philspel.c
        philspel.h
        sampleDictionary
        sampleInput
        sampleOutput
        testOutput)

add_executable(proj1 ${SOURCE_FILES})