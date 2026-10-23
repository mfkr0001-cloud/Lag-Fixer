
cmake_minimum_required(VERSION 3.21)

project(LagReducer VERSION 1.2.2)

# Workaround for fmt consteval errors with Clang 21
add_compile_definitions(FMT_USE_CONSTEVAL=0)

# Load the Geode SDK
if (DEFINED ENV{GEODE_SDK})
    add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
else()
    message(FATAL_ERROR "GEODE_SDK environment variable is not set!")
endif()

# Mod source files
file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

setup_geode_mod(${PROJECT_NAME})
