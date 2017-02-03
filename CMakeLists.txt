cmake_minimum_required(VERSION 3.6)
project(Berkeley_CS61C)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        proj1/hashtable.c
        proj1/hashtable.h
        proj1/philspel.c
        proj1/philspel.h
        proj1/sampleDictionary
        proj1/sampleInput
        proj1/sampleOutput
        proj1/testOutput
        README.md lab1/hello.c)

add_executable(Berkeley_CS61C ${SOURCE_FILES})