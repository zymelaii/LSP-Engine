cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
	LSP-Engine
	VERSION 0.1.0
	DESCRIPTION "Less than Shit Physics Engine"
	HOMEPAGE_URL "https://github.com/zymelaii/LSP-Engine"
	LANGUAGES C CXX
)

add_subdirectory(src)
add_subdirectory(example)
