cmake_minimum_required(VERSION 3.23)
project(ttfb DESCRIPTION "cli utility, which use lib" LANGUAGES C)

add_executable(
    main
    src/main.cpp
)
target_compile_features(main PRIVATE c_std_99)
target_link_libraries(main PUBLIC ttfb)
