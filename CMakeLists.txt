cmake_minimum_required(VERSION 3.31)
project(file_read)

set(CMAKE_CXX_STANDARD 23)

add_library(file_read STATIC
        file_deserialising.h
        buffer.cpp
        buffer.h)
