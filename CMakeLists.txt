cmake_minimum_required(VERSION 3.21)
project(ex5_jonathan_zl)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(presubmit
        Movie.cpp
        Movie.h
        presubmit.cpp
        RecommendationSystem.cpp
        RecommendationSystem.h
        RecommendationSystemLoader.cpp
        RecommendationSystemLoader.h
        User.cpp
        User.h
        UsersLoader.cpp
        UsersLoader.h)

add_executable(print_main
        Movie.cpp
        print_main.cpp
        RecommendationSystem.cpp
        RecommendationSystemLoader.cpp
        User.cpp
        UsersLoader.cpp)