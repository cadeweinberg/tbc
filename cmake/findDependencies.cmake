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

include(FetchContent)

set(BOOST_INCLUDE_LIBRARIES program_options test leaf random)
set(BOOST_ENABLE_CMAKE ON)

FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.90.0/boost-1.90.0-cmake.zip
    URL_HASH SHA256=89dc8dbdd61b0212dfff5286a07227cc40d0243f344037739b7c7f7f4a6c3584
)

#set(BOOST_INCLUDE_LIBRARIES program_options test CACHE STRING "")

#FetchContent_Declare(
#    re2c
#    GIT_REPOSITORY https://github.com/skvadrik/re2c.git
#    GIT_TAG 8a44906f5c5df26138d2202c335bf9a9f5f2fefe
#)

FetchContent_MakeAvailable(Boost)

#message(STATUS "re2c available ${re2c_BINARY_DIR}")

find_program(RE2C re2c REQUIRED
    DOC "The generator program for the Lexer"
)

if (re2c_FOUND)
    message(STATUS "re2c found: ${RE2C}")
endif ()

