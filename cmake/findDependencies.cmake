#
# Copyright 2026 Cade Weinberg
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the “Software”), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#

cmake_minimum_required (VERSION 3.28)

find_package(Boost CONFIG 
    COMPONENTS
    unit_test_framework
    program_options
)

if (Boost_FOUND)
    message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
else ()
    message(STATUS "Boost not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG 1bed2b0712b2119f20d66c5053def9173c8462a5
    )

    # Configure Boost to only build what we need
    set(BOOST_INCLUDE_LIBRARIES program_options test CACHE STRING "")
    set(BOOST_ENABLE_CMAKE ON CACHE BOOL "")

    # Enable __declspec attribute support for Emscripten
    if(EMSCRIPTEN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions" CACHE STRING "")
    endif()

    FetchContent_MakeAvailable(Boost)
    # I don't understand why this is necessary, given that the headers target includes 
    # leaf when we are using the system level install.
    if(TARGET Boost::headers AND NOT TARGET Boost::headers_with_leaf)
        add_library(boost_headers_with_leaf INTERFACE)
        add_library(Boost::headers_with_leaf ALIAS boost_headers_with_leaf)
        target_link_libraries(boost_headers_with_leaf INTERFACE Boost::headers)
        target_include_directories(boost_headers_with_leaf INTERFACE 
            "${boost_SOURCE_DIR}/libs/leaf/include"
            "${boost_SOURCE_DIR}/libs/random/include"
            "${boost_SOURCE_DIR}/libs/integer/include"
        )
    endif()
endif ()

find_program(RE2C re2c REQUIRED
    DOC "The generator program for the Lexer"
)

if (RE2C_FOUND)
    message(STATUS "re2c found: ${RE2C}")
endif ()

