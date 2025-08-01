# SPDX-License-Identifier: MIT
#
# Copyright 2025 dgz0
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the “Software”), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# The FATAL_ERROR option is accepted but ignored by CMake 2.6 and higher. It
# should be specified so CMake versions 2.4 and lower fail with an error instead
# of just a warning.
cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

project(lp-mspm0g3507
        VERSION 1.0.0.0
        DESCRIPTION "LP-MSPM0G3507 Playground"
        HOMEPAGE_URL "https://github.com/dgz0/lp-mspm0g3507"
        LANGUAGES C
)

if (NOT CMAKE_TOOLCHAIN_FILE)
        message(FATAL_ERROR
                "A toolchain file is required to be specified to build this "
                "project. If you are using Visual Studio Code or JetBrains "
                "CLion, this should've already been handled for you via "
                "CMakePresets.json. If not, pass the desired preset manually.")
endif()

if (CMAKE_VERSION LESS 3.26)
        # CMake versions >=3.26 treat this property as ON by default.
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

add_subdirectory(src)
