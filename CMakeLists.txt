cmake_minimum_required(VERSION 3.21)
project(LagReducer VERSION 1.3.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compatibility define retained from the previously successful CI setup.
add_compile_definitions(FMT_USE_CONSTEVAL=0)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK. Define GEODE_SDK to point to your installed Geode SDK.")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
setup_geode_mod(${PROJECT_NAME})
