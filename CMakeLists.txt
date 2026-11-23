cmake_minimum_required(VERSION 3.12)
project(QuizArena VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(QuizArena src/main.cpp)

if(MSVC)
    target_compile_options(QuizArena PRIVATE /W4 /EHsc /utf-8)
else()
    target_compile_options(QuizArena PRIVATE -Wall -Wextra -pedantic)
endif()
