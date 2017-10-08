# Copyright 2017 Tymoteusz Blazejczyk
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.1)
project(json CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

if (CMAKE_TOOLCHAIN_FILE AND CMAKE_SYSTEM_NAME MATCHES Generic)
    option(MEMORY_CHECK "Enable/disable memory checks" OFF)
    option(EXCEPTIONS "Enable/disable exceptions" OFF)
    option(RTTI "Enable/disable rtti" OFF)
    option(THREADS "Enable/disable threads" OFF)
else()
    option(MEMORY_CHECK "Enable/disable memory checks" ON)
    option(EXCEPTIONS "Enable/disable exceptions" ON)
    option(RTTI "Enable/disable rtti" ON)
    option(THREADS "Enable/disable threads" ON)
endif()

if (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
    option(LTO "Enable/disable link time optimization" ON)
else()
    option(LTO "Enable/disable link time optimization" OFF)
endif()

option(TESTS "Enable/disable tests" ON)
option(EXAMPLES "Enable/disable examples" ON)
option(WARNINGS_INTO_ERRORS "Enable/disable warnings as errors" OFF)

include(AddCodeCoverage)
include(AddGnuCompiler)
include(AddMsvcCompiler)
include(AddClangCompiler)
include(AddThreads)
include(AddTestRunner)

include_directories(include)

install(DIRECTORY include/json DESTINATION include FILES_MATCHING PATTERN "*.hpp")

add_subdirectory(src)

if (EXAMPLES)
    add_subdirectory(examples)
endif()

if (TESTS)
    #enable_testing()
    #add_subdirectory(tests)
endif()
