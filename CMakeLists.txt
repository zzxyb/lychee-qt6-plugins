cmake_minimum_required(VERSION 3.16)

if(CMAKE_VERSION VERSION_LESS "3.16")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

project(lychee-qt6-plugins VERSION 0.1 LANGUAGES CXX)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII)
add_definitions(-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT)

add_subdirectory(decoration-plugin)
add_subdirectory(platformtheme-plugin)

