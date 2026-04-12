cmake_minimum_required(VERSION 3.30)
project(basedvt VERSION 0.1)

option(BASEDVT_BUILD_DEMOS "Build basedvt demos" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES FALSE)

add_subdirectory(lib)

if(BASEDVT_BUILD_DEMOS)
	add_subdirectory(demo)
endif()
