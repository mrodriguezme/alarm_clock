# SPDX-License-Identifier: MIT
#
# Copyright 2026 Michael Rodriguez
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set(CMAKE_SYSTEM_NAME Generic-ELF)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_program(
	CMAKE_C_COMPILER
	NAMES arm-none-eabi-gcc
	REQUIRED
)

find_program(
	CMAKE_ASM_COMPILER
	NAMES arm-none-eabi-gcc
	REQUIRED
)

find_program(
	CMAKE_AR
	NAMES arm-none-eabi-ar
	REQUIRED
)

find_program(
	CMAKE_RANLIB
	NAMES arm-none-eabi-ranlib
	REQUIRED
)

find_program(
	CMAKE_OBJCOPY
	NAMES arm-none-eabi-objcopy
	REQUIRED
)

find_program(
	CMAKE_SIZE_UTIL
	NAMES arm-none-eabi-size
	REQUIRED
)

execute_process(
        COMMAND ${CMAKE_C_COMPILER} -print-sysroot
        OUTPUT_VARIABLE TOOLCHAIN_SYSROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CMAKE_SYSROOT ${TOOLCHAIN_SYSROOT})

set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(COMPILER_FLAGS
	--crt0=minimal
	--printf=m
	--scanf=m
	-Wl,--gc-sections
	-Os
	-Wall
	-Wextra
	-fdata-sections
	-ffunction-sections
	-mcpu=cortex-m0plus
	-specs=picolibc.specs
	-std=gnu23
)

list(JOIN COMPILER_FLAGS " " COMPILER_FLAGS)

set(CMAKE_C_FLAGS_DEBUG_INIT "${COMPILER_FLAGS} -ggdb3")

set(
	CMAKE_C_FLAGS_DEBUG
	"${CMAKE_C_FLAGS_DEBUG_INIT}"
	CACHE STRING "" FORCE
)

set(CMAKE_C_FLAGS_RELEASE_INIT "${COMPILER_FLAGS} -DNDEBUG")

set(
	CMAKE_C_FLAGS_RELEASE
	"${CMAKE_C_FLAGS_RELEASE_INIT}"
	CACHE STRING "" FORCE
)

function(toolchain_set_target_linker_script TARGET SCRIPT)
	target_link_options(${TARGET} INTERFACE "SHELL:-T ${SCRIPT}")
endfunction()

function(toolchain_set_target_map_file TARGET MAP_FILE)
	target_link_options(${TARGET} PRIVATE "LINKER:-Map=${MAP_FILE}")
endfunction()

function(toolchain_print_target_size TARGET)
	add_custom_command(
		TARGET ${TARGET}
		POST_BUILD COMMAND
		${CMAKE_SIZE_UTIL} -B $<TARGET_FILE:${TARGET}>
	)
endfunction()
