cmake_minimum_required(VERSION 3.31)
project(file_read)

set(CMAKE_CXX_STANDARD 23)

add_library(file_read INTERFACE
        src/file_deserialising.h
        src/buffer.h
        src/file_read.h)

set_target_properties(file_read PROPERTIES LINKER_LANGUAGE CXX)
