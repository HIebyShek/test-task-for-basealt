cmake_minimum_required(VERSION 3.23)
project(tests)

add_executable(
    tests
    main.test.cpp
    ttfb.test.cpp
)
target_compile_features(tests PRIVATE cxx_std_11)
target_link_libraries(tests PRIVATE doctest)
target_link_libraries(tests PRIVATE ttfb)