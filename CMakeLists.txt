cmake_minimum_required(VERSION 3.14.0)

project(base-server VERSION 1.0.2 LANGUAGES CXX)

include(./cmake/define.cmake)
include(./cmake/function.cmake)

add_subdirectory(src)
add_subdirectory(external_project)

if(ENABLE_TEST)
	func_make_coverage()
endif(ENABLE_TEST)
