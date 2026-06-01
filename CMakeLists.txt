# WARNING:
# This build system is already a mess... CMake is horrible.
# The problem is that I really don't want the binaries to be more that 100MB
# And apparently CMake insists on making the binary (through hidden compile flags)
# over 100MB. If I don't do anything at all that goes for both Debug AND Release.
# Hopefully this won't need much modification into the future.

cmake_minimum_required(VERSION 3.6)
project(godot-openmpt)

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(GODOT_GDEXTENSION_DIR ../godot-cpp/gdextension/include CACHE STRING "Path to GDExtension interface header directory")
set(CPP_BINDINGS_PATH ../godot-cpp/ CACHE STRING "Path to C++ bindings")
set(GODOT_LIB_PATH ../godot-cpp/bin CACHE STRING "Path to the built Godot C++ libraries")
set(GODOT_GEN_INCLUDE_PATH ../godot-cpp/gen/include CACHE STRING "Path to generated Godot includes")
set(EXAMPLE_DIR ${CMAKE_SOURCE_DIR}/example/ CACHE STRING "Path to example project")

# Change the output directory to the bin directory
set(BUILD_PATH ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILD_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILD_PATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BUILD_PATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BUILD_PATH}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${BUILD_PATH}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${BUILD_PATH}")

# Set the c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GODOT_COMPILE_FLAGS )
set(GODOT_LINKER_FLAGS )

# Get OpenMPT Sources
file(GLOB LIBOPENMPT_SOURCES openmpt/libopenmpt/*.cpp)
file(GLOB_RECURSE OPENMPT_SOURCES openmpt/sounddsp/*.cpp** openmpt/soundlib/*.cpp** openmpt/common/*.cpp**)

# Define the libopenmpt library
add_library(libopenmpt STATIC
	${LIBOPENMPT_SOURCES} ${OPENMPT_SOURCES})

if(CMAKE_BUILD_TYPE MATCHES Debug)
	target_compile_definitions(libopenmpt
		PRIVATE
			MPT_BUILD_DEBUG)
endif()

target_compile_definitions(libopenmpt
	PRIVATE
		LIBOPENMPT_BUILD)

target_include_directories(libopenmpt SYSTEM
	PUBLIC
		openmpt
	PRIVATE
		openmpt/libopenmpt
		openmpt/common
		openmpt/sounddsp
		openmpt/soundlib
		openmpt/src
)

# Define the Godot Extension library
add_library(${PROJECT_NAME} SHARED
	src/register_types.cpp
	src/audio_stream_mpt.cpp
    src/openmpt_singleton.cpp
)

target_include_directories(${PROJECT_NAME} 
	PRIVATE
		${CPP_BINDINGS_PATH}/include
		${GODOT_GEN_INCLUDE_PATH}
		${GODOT_GDEXTENSION_DIR}
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
	set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /WX") # /GF /MP
	set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /DTYPED_METHOD_BIND")
	set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /EHsc")

	if(CMAKE_BUILD_TYPE MATCHES Debug)
		set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /MDd") # /Od /RTC1 /Zi
	else()
		set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /MD /O2") # /Oy /GL /Gy
		string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
		string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
	endif(CMAKE_BUILD_TYPE MATCHES Debug)

	# Disable conversion warning, truncation, unreferenced var, signed mismatch
	set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /wd4244 /wd4305 /wd4101 /wd4018 /wd4267")

	# libopenmpt compiler definitions
	target_compile_definitions(libopenmpt
		PRIVATE
			MPT_BUILD_MSVC
			WIN32
			NOMINMAX
			_CRT_NONSTDC_NO_WARNINGS
			_CRT_SECURE_NO_WARNINGS
			_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1
			_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1
			_WIN32_WINNT=0x0A00
			NTDDI_VERSION=0x0A00000A)
else()
	# STOP adding -g when I DON'T WANT -g!!!
	set(CMAKE_CXX_FLAGS_DEBUG "")
	set(CMAKE_C_FLAGS_DEBUG "")

	if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		set(GODOT_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Wl,-flto,--gc-sections,-R,'$$ORIGIN'")
		set(CMAKE_AR "gcc-ar")
		set(CMAKE_NM "gcc-nm")
		set(CMAKE_RANLIB "gcc-ranlib")
	endif()

	if(${PLATFORM} MATCHES "web")
		set(GODOT_LINKER_FLAGS "-sWASM_BIGINT -sUSE_PTHREADS=1 -sSIDE_MODULE=1 -sSUPPORT_LONGJMP='wasm' ")
		set(CMAKE_C_COMPILER "emcc")
		set(CMAKE_CXX_COMPILER "em++")
		set(CMAKE_AR "emar")
		set(CMAKE_NM "emnm")
		set(CMAKE_RANLIB "emranlib")
		set(CMAKE_SHARED_LIBRARY_SUFFIX ".wasm")
	endif()

	if(${PLATFORM} MATCHES "windows")
		set(GODOT_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Wl,-flto,--gc-sections,-R,'$$ORIGIN',-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
		set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
		set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-c++")
		set(CMAKE_AR "x86_64-w64-mingw32-gcc-ar")
		set(CMAKE_NM "x86_64-w64-mingw32-gcc-nm")
		set(CMAKE_RANLIB "x86_64-w64-mingw32-gcc-ranlib")	
		set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")
  endif()

	set(GODOT_COMPILE_FLAGS "-fPIC -flto -fvisibility=hidden -fvisibility-inlines-hidden -ffunction-sections -fdata-sections -fexceptions -frtti -pthread")

	if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
		set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -O3")
	endif()
endif()

# Add the compile flags
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS ${GODOT_COMPILE_FLAGS})
set_property(TARGET libopenmpt APPEND_STRING PROPERTY COMPILE_FLAGS ${GODOT_COMPILE_FLAGS})
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS ${GODOT_LINKER_FLAGS})
set_property(TARGET libopenmpt APPEND_STRING PROPERTY LINK_FLAGS ${GODOT_LINKER_FLAGS})

if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" AND CMAKE_BUILD_TYPE MATCHES Debug)
	

	# Only generate debug symbols for the main extension... NOT libopenmpt. 
	# Otherwise the binary sizes will be out of control. 
	# Comment this out and compile if you really need to debug what libopenmpt is doing 
	# (you probably don't)
	#set_property(TARGET libopenmpt APPEND_STRING PROPERTY COMPILE_FLAGS "-fno-omit-frame-pointer -O0 -g")
	set_property(TARGET libopenmpt APPEND_STRING PROPERTY COMPILE_FLAGS " -O3") # If we don't want debug symbols, may as well -O3

	set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS " -fno-omit-frame-pointer -O0 -g")
endif()

# Create the correct name (godot.os.build_type.system_bits)
# Synchronized with godot-cpp's CMakeLists.txt

set(BITS .x86_32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BITS .x86_64)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

if(${PLATFORM} MATCHES "web")
	set(BITS .wasm32)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(GODOTCPP_BUILD_TYPE template_debug)
	set(BUILD_TYPE debug)
else()
	set(GODOTCPP_BUILD_TYPE template_release)
	set(BUILD_TYPE release)
endif()

string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME)

if(ANDROID)
	# Added the android abi after system name
	set(SYSTEM_NAME ${SYSTEM_NAME}.${ANDROID_ABI})
	set(BITS "")
endif()

if(${PLATFORM} MATCHES "web")
	set(SYSTEM_NAME web)
endif()

if(${PLATFORM} MATCHES "windows")
	set(SYSTEM_NAME windows)
endif()

target_link_directories(${PROJECT_NAME}
	PRIVATE
		${GODOT_LIB_PATH}
)

target_link_libraries(${PROJECT_NAME}
	godot-cpp.${SYSTEM_NAME}.${GODOTCPP_BUILD_TYPE}${BITS}
	libopenmpt
)

set(MAIN_EXT_OUTPUT_NAME libgdmpt-${SYSTEM_NAME}.${BUILD_TYPE}${BITS})
set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME ${MAIN_EXT_OUTPUT_NAME})

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_PATH}/${MAIN_EXT_OUTPUT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_SOURCE_DIR}/addons/godot-openmpt/bin/${MAIN_EXT_OUTPUT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${EXAMPLE_DIR}/addons/godot-openmpt
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/addons/godot-openmpt ${EXAMPLE_DIR}/addons/godot-openmpt
	DEPENDS $<TARGETFILE:${PROJECT_NAME}>
)

