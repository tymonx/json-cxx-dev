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

if (CMAKE_BUILD_TYPE MATCHES "Coverage")
    get_filename_component(COMPILER_ROOT ${CMAKE_C_COMPILER} DIRECTORY)
    get_filename_component(COMPILER_NAME ${CMAKE_C_COMPILER} NAME)

    if (CMAKE_C_COMPILER_ID MATCHES Clang)
        string(REGEX REPLACE "([a-z0-9-]*)clang.*" "\\1"
            COMPILER_PREFIX ${COMPILER_NAME})

        string(REGEX REPLACE ".*clang([a-z0-9-]*)" "\\1"
            COMPILER_SUFFIX ${COMPILER_NAME})

        find_program(GCOV_COMMAND
            ${COMPILER_PREFIX}llvm-cov${COMPILER_SUFFIX}
            llvm-cov${COMPILER_SUFFIX}
            ${COMPILER_PREFIX}llvm-cov
            llvm-cov
            HINTS ${COMPILER_ROOT}
        )

        if (GCOV_COMMAND)
            file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
            set(LLVM_GCOV_SCRIPT llvm-gcov.sh)

            file(WRITE ${CMAKE_BINARY_DIR}/${LLVM_GCOV_SCRIPT}
                "#!/bin/sh\n"
                "exec ${GCOV_COMMAND} gcov \"$@\"\n"
            )

            file(INSTALL ${CMAKE_BINARY_DIR}/${LLVM_GCOV_SCRIPT}
                DESTINATION ${CMAKE_BINARY_DIR}/bin
                FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)

            set(GCOV_COMMAND ${CMAKE_BINARY_DIR}/bin/llvm-gcov.sh)
        endif()
    elseif (CMAKE_C_COMPILER_ID MATCHES GNU)
        string(REGEX REPLACE "([a-z0-9-]*)gcc.*" "\\1"
            COMPILER_PREFIX ${COMPILER_NAME})

        string(REGEX REPLACE ".*gcc([a-z0-9-]*)" "\\1"
            COMPILER_SUFFIX ${COMPILER_NAME})

        find_program(GCOV_COMMAND
            ${COMPILER_PREFIX}gcov${COMPILER_SUFFIX}
            gcov${COMPILER_SUFFIX}
            ${COMPILER_PREFIX}gcov
            gcov
            HINTS ${COMPILER_ROOT}
        )
    else()
        message(FATAL_ERROR
            "Code coverage is only supported by Clang and GNU compiler")
    endif()

    if (NOT GCOV_COMMAND)
        message(FATAL_ERROR "Cannot find gcov command")
    endif()

    message(STATUS "Found gcov: ${GCOV_COMMAND}")

    find_program(LCOV_COMMAND lcov)
    if (NOT LCOV_COMMAND)
        message(FATAL_ERROR "Cannot find lcov command")
    endif()

    message(STATUS "Found lcov: ${LCOV_COMMAND}")

    find_program(GENHTML_COMMAND genhtml)
    if (NOT GENHTML_COMMAND)
        message(FATAL_ERROR "Cannot find genhtml command")
    endif()

    message(STATUS "Found genhtml: ${GENHTML_COMMAND}")

    add_custom_target(code_coverage
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
        COMMAND ${LCOV_COMMAND}
            --zerocounters
            --directory .
        COMMAND ${LCOV_COMMAND}
            --capture
            --initial
            --gcov-tool ${GCOV_COMMAND}
            --base-directory ${CMAKE_SOURCE_DIR}
            --directory .
            --output-file coverage.base
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target test
        COMMAND ${LCOV_COMMAND}
            --capture
            --gcov-tool ${GCOV_COMMAND}
            --base-directory ${CMAKE_SOURCE_DIR}
            --directory .
            --output-file coverage.run
        COMMAND ${LCOV_COMMAND}
            --add-tracefile coverage.base
            --add-tracefile coverage.run
            --output-file coverage.total
        COMMAND ${LCOV_COMMAND}
            --gcov-tool ${GCOV_COMMAND}
            --extract coverage.total */src/*.cpp
            --extract coverage.total */src/*.hpp
            --extract coverage.total */include/utest/*.hpp
            --output-file coverage.info
        COMMAND ${GENHTML_COMMAND}
            --legend
            --title "µTest - unit testing framework"
            --demangle-cpp
            --output-directory coverage
            coverage.info
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    message(STATUS "Enabled code coverage support")
else ()
    message(STATUS "Disabled code coverage support")
endif()
