cmake_minimum_required(VERSION 3.3)
project(warlight_starterbot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    tools/noncopyable.hpp
    tools/StringManipulation.h
    Bot.cpp
    Bot.h
    main.cpp
    main.h
    Parser.cpp
    Parser.h
    Region.cpp
    Region.h
    SuperRegion.cpp
    SuperRegion.h)

add_executable(warlight_starterbot ${SOURCE_FILES})