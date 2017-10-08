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

if (NOT TESTS)
    return()
endif()

set(TEST_RUNNER "")

if (CMAKE_TOOLCHAIN_FILE AND CMAKE_SYSTEM_NAME MATCHES "Generic")
    unset(TEST_RUNNER)

    if (CMAKE_SYSTEM_PROCESSOR MATCHES "cortex*")
        find_program(QEMU_ARM_COMMAND qemu-arm)

        if (QEMU_ARM_COMMAND)
            message(STATUS "Tests will be run through ${QEMU_ARM_COMMAND}")
            set(TEST_RUNNER ${QEMU_ARM_COMMAND})
        endif()
    endif()
elseif (MEMORY_CHECK)
    find_program(VALGRIND_COMMAND valgrind)

    if (VALGRIND_COMMAND)
        message(STATUS "Tests will be run through ${VALGRIND_COMMAND}")
        set(TEST_RUNNER ${VALGRIND_COMMAND}
            -v
            --tool=memcheck
            --leak-check=full
            --show-reachable=yes
            --track-origins=yes
            --error-exitcode=1
        )
    endif()
endif()

if (DEFINED TEST_RUNNER AND NOT TEST_RUNNER)
    message(STATUS "Tests will be run normally")
elseif (NOT DEFINED TEST_RUNNER)
    message(WARNING "Tests cannot be run")
endif()
