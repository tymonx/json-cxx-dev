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

add_subdirectory(allocator)
add_subdirectory(unicode)

add_library(json-core OBJECT
    #    array.cpp
    #array_iterator.cpp
    #object.cpp
    #object_iterator.cpp
    #pair.cpp
    #value.cpp
    #value_iterator.cpp
    #list.cpp
    #number.cpp
    #list_iterator.cpp
    #string.cpp
    #parser.cpp
    #string_view.cpp
    #allocator.cpp
    string_base.cpp
    string_view.cpp
    string_iterator.cpp
    string_reverse_iterator.cpp
)

if (NOT JSON_ALLOCATOR_TYPE)
    if (THREADS)
        set(JSON_ALLOCATOR_TYPE JSON_ALLOCATOR_CONCURRENT_BLOCK)
    elseif (CMAKE_TOOLCHAIN_FILE AND CMAKE_SYSTEM_NAME MATCHES Generic)
        set(JSON_ALLOCATOR_TYPE JSON_ALLOCATOR_POOL)
    else()
        set(JSON_ALLOCATOR_TYPE JSON_ALLOCATOR_BLOCK)
    endif()
endif()

set_source_files_properties(allocator.cpp PROPERTIES
    COMPILE_DEFINITIONS ${JSON_ALLOCATOR_TYPE})

add_library(json STATIC
    $<TARGET_OBJECTS:json-core>
    $<TARGET_OBJECTS:json-unicode>
    $<TARGET_OBJECTS:json-allocator>
)

set_target_properties(json PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    set_source_files_properties(allocator.cpp PROPERTIES
        COMPILE_FLAGS -Wno-exit-time-destructors)
elseif (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set_source_files_properties(string_base.cpp PROPERTIES
        COMPILE_FLAGS -Wno-conversion)
endif()
