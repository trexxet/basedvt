cmake_minimum_required(VERSION 3.30)
project(basedvt VERSION 0.1)

option(BASEDVT_BUILD_TESTS "Build basedvt tests" OFF)
option(BASEDVT_BUILD_DEMOS "Build basedvt demos" OFF)
option(BASEDVT_ENABLE_LOG "Enable debug logging" OFF)
option(BASEDVT_ENABLE_TO_STRING "Enable to_string() methods" ${PROJECT_IS_TOP_LEVEL})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES FALSE)

if(MINGW)
	add_compile_options(-fno-ms-extensions)
endif()

add_subdirectory(lib)

if(BASEDVT_BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()

if(BASEDVT_BUILD_DEMOS)
	add_subdirectory(demo)
endif()
