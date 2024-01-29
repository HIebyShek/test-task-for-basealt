cmake_minimum_required(VERSION 3.23)
project(libttfb LANGUAGES C)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_library(
    ttfb SHARED
    src/ttfb.c
    src/package.c
    src/version.c
)
target_include_directories(ttfb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(ttfb PRIVATE c_std_99)

target_link_libraries(ttfb PRIVATE curl)

find_package(json-c REQUIRED)
target_link_libraries(ttfb PUBLIC json-c::json-c)



