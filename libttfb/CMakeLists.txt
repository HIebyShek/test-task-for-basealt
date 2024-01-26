cmake_minimum_required(VERSION 3.23)
project(libttfb LANGUAGES C)

add_library(
    ttfb SHARED
    src/ttfb.c
)
target_include_directories(ttfb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(ttfb PRIVATE c_std_99)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)


