cmake_minimum_required(VERSION 4.1)
project(anime_recommender_cpp)

set(CMAKE_CXX_STANDARD 26)

add_executable(anime_recommender_cpp main.cpp
        Recommender.cpp
        Recommender.h)
