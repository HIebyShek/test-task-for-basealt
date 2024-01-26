cmake_minimum_required(VERSION 3.23)
project(external)

if (TTFB_BUILD_TESTS)
  file(GLOB DOCTEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/doctest/*)
  list(LENGTH DOCTEST_FILES DOCTEST_FILES_NUM)

  if(DOCTEST_FILES_NUM EQUAL 0)
    message(FATAL_ERROR "Clone doctest submodule for enable testing."
                        "\nCall from project root:"
                        "\n    git submodule update --init external/doctest")
  else()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/doctest)
  endif()
endif()
