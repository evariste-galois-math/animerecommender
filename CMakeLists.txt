cmake_minimum_required(VERSION 4.1)
project(anime_recommender_cpp)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/curl;/opt/homebrew/opt/nlohmann-json;/opt/homebrew/opt/sqlite")
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(anime_recommender_cpp main.cpp
        Recommender.cpp
        Recommender.h
        JikanClient.cpp
        JikanClient.h
        Database.cpp
        Database.h
        Server.cpp
        Server.h)

target_link_libraries(anime_recommender_cpp PRIVATE CURL::libcurl nlohmann_json::nlohmann_json SQLite::SQLite3)

add_executable(anime_recommender_server server.cpp
        Recommender.cpp
        Recommender.h
        JikanClient.cpp
        JikanClient.h
        Database.cpp
        Database.h
        Server.cpp
        Server.h)

target_link_libraries(anime_recommender_server PRIVATE CURL::libcurl nlohmann_json::nlohmann_json SQLite::SQLite3)