cmake_minimum_required(VERSION 3.30)
project(LinkShortener)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system filesystem)

find_package(nlohmann_json REQUIRED)

add_executable(link_shortener link_shortener.cpp link_shortener.h
        main.cpp
        link_shortener.cpp
        link_shortener.h
)

target_link_libraries(link_shortener nlohmann_json::nlohmann_json)

target_link_libraries(link_shortener Boost::system Boost::filesystem)
