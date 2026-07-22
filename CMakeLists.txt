cmake_minimum_required(VERSION 4.1)
project(anime_recommender_cpp)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/curl")
find_package(CURL REQUIRED)

add_executable(anime_recommender_cpp main.cpp
        Recommender.cpp
        Recommender.h
        JikanClient.cpp
        JikanClient.h)

target_link_libraries(anime_recommender_cpp PRIVATE CURL::libcurl)