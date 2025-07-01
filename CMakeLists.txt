cmake_minimum_required(VERSION 3.31)
project(file_read)

set(CMAKE_CXX_STANDARD 23)

add_library(file_read STATIC
        src/file_deserialising.h
        src/buffer.cpp
        src/buffer.h
        src/file_read.h)
