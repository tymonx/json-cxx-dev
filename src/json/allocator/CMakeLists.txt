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

set(CXX_SOURCES
    block.cpp
    pool.cpp
    standard.cpp
    dummy.cpp
)

if (THREADS)
    set(CXX_SOURCES ${CXX_SOURCES}
        concurrent_block.cpp
    )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    set_source_files_properties(dummy.cpp PROPERTIES
        COMPILE_FLAGS -Wno-exit-time-destructors)
endif()

add_library(json-allocator OBJECT ${CXX_SOURCES})
