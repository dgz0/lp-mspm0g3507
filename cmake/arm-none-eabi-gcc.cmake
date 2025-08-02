# This toolchain file is heavily inspired by the arm-cmake-toolchains repository
# located at https://github.com/vpetrigo/arm-cmake-toolchains/.
#
# BSD 3-Clause License
#
# Copyright (c) 2017, Vladimir Petrigo
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

set(CMAKE_SYSTEM_NAME Generic-ELF)
set(CMAKE_SYSTEM_PROCESSOR ARM)

find_program(BINUTILS_PATH arm-none-eabi-gcc NO_CACHE)

if (NOT BINUTILS_PATH)
        message(FATAL_ERROR
                "ARM GCC toolchain not found! Make sure that the ARM GCC "
                "toolchain is in your PATH.")
endif()

get_filename_component(ARM_TOOLCHAIN_DIR ${BINUTILS_PATH} DIRECTORY)

# Without this flag, CMake is not able to pass the compilation check.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Use newlib-nano; it is optimized for size and omits some exotic features that
# are not likely to be used.
set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=nano.specs -mcpu=cortex-m0plus -mthumb")

set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR           arm-none-eabi-gcc-ar)
set(CMAKE_RANLIB       arm-none-eabi-gcc-ranlib)

execute_process(
        COMMAND arm-none-eabi-gcc -print-sysroot
        OUTPUT_VARIABLE ARM_GCC_SYSROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# These flags are set regardless of the build type.
set(STANDARD_FLAGS
        -mcpu=cortex-m0plus
        -march=armv6-m
        -mthumb
        -ffunction-sections
        -fdata-sections
        -Wall
        -Wextra
        -ggdb3
        -Os
        -std=gnu17
        -Wl,--gc-sections,--Map=output.map
)

list(JOIN STANDARD_FLAGS " " STANDARD_FLAGS)

set(CMAKE_C_FLAGS_DEBUG_INIT ${STANDARD_FLAGS})

set(
        CMAKE_C_FLAGS_DEBUG
        "${CMAKE_C_FLAGS_DEBUG_INIT}"
        CACHE STRING "" FORCE
)

set(CMAKE_C_FLAGS_RELEASE_INIT "-Os -Wall")

set(
        CMAKE_C_FLAGS_RELEASE
        "${CMAKE_C_FLAGS_RELEASE_INIT}"
        CACHE STRING "" FORCE
)

set(
        CMAKE_OBJCOPY
        ${ARM_TOOLCHAIN_DIR}/arm-none-eabi-objcopy
        CACHE INTERNAL "objcopy tool"
)

set(
        CMAKE_SIZE_UTIL
        ${ARM_TOOLCHAIN_DIR}/arm-none-eabi-size
        CACHE INTERNAL "size tool"
)

set(CMAKE_SYSROOT ${ARM_GCC_SYSROOT})

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Add linker script to a target.
function(set_linker_script TARGET SCRIPT_PATH)
        string(APPEND CMAKE_EXE_LINKER_FLAGS " -T ${SCRIPT_PATH}")

        get_target_property(LIST_LINK_DEPS ${TARGET} LINK_DEPENDS)

        if (LIST_LINK_DEPS)
                string(APPEND LIST_LINK_DEPS " ${SCRIPT_PATH}")
        else()
                set(LIST_LINK_DEPS ${SCRIPT_PATH})
        endif()

        set_target_properties(
                ${TARGET} PROPERTIES
                LINK_DEPENDS ${LIST_LINK_DEPS}
        )
endfunction()

function(print_firmware_size TARGET)
        add_custom_command(
                TARGET ${TARGET}
                POST_BUILD
                COMMAND ${CMAKE_SIZE_UTIL} -B "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}${CMAKE_EXECUTABLE_SUFFIX}"
        )
endfunction()
