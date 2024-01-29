cmake_minimum_required(VERSION 3.23)
project(ttfb DESCRIPTION "cli utility, which use lib" LANGUAGES C)

add_executable(
    main
    src/main.c
    src/cli.c
    src/method.c
)
target_compile_features(main PRIVATE c_std_99)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(main PUBLIC ttfb)
