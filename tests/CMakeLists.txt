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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

function (add_json_test target_name)
    add_executable(test_${target_name} test_${target_name}.cpp)
    target_link_libraries(test_${target_name} json gtest gtest_main)
    add_test(test_${target_name} test_${target_name})

    if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
        set_source_files_properties(test_${target_name}.cpp
            PROPERTIES COMPILE_FLAGS
            "-Wno-global-constructors -Wno-zero-as-null-pointer-constant"
        )
    endif()
endfunction()

add_json_test(string)
