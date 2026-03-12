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

# build the supporting libraries

include(${TBC_CMAKE_DIR}/generate_lexer.cmake)

add_library(tbc_input)

set (TBC_INPUT_SOURCE_LIST
    ${TBC_SOURCE_DIR}/input/lexer.cpp
    ${TBC_SOURCE_DIR}/input/token.cpp
)
target_sources(tbc_input PUBLIC
    ${TBC_INPUT_SOURCE_LIST}
)

target_include_directories(tbc_input PUBLIC
    ${TBC_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)


# Build the tbc executable

add_executable(tbc)

target_sources(tbc PRIVATE
    ${TBC_SOURCE_DIR}/main.cpp
)

target_include_directories(tbc PRIVATE
    ${TBC_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

