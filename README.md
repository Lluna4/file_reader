This is an abstraction to read files in c++ (in a very early state)

It's more or less based on my networking lib https://github.com/Lluna4/netlib

It's primarily built to be used for a png reader for https://forgejo.luna4.xyz/Luna/vulkan_renderer

# Features
For now it has a simple file reader that gives you stuff already deserialized (for now only supports numerals), it buffers reads so the performance is good and the deserialization is in compilation time

# How to build
It uses cmake as a build system (this project requires a c++23 compatible compiler), to compile `mkdir build && cd build` and then `cmake ..` and `cmake --build .` most of things are header only (except buffer.h and buffer.cpp)