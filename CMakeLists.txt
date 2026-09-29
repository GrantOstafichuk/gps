cmake_minimum_required(VERSION 3.16)

project(GPXReader
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(GPXReader
    main.cpp
    tinyxml2.cpp
)

target_include_directories(GPXReader PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)