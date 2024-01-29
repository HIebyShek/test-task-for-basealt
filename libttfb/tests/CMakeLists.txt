cmake_minimum_required(VERSION 3.23)
project(test_libttfb)

add_executable(
    test_libttfb
    main.test.cpp
    ttfb.test.cpp
    package.test.cpp
    version.test.cpp
)
target_compile_features(test_libttfb PRIVATE cxx_std_11)
target_link_libraries(test_libttfb PRIVATE doctest)
target_link_libraries(test_libttfb PRIVATE ttfb)