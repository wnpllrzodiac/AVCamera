
cmake_minimum_required(VERSION 2.8.3)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_COMPILER_IS_CLANGXX 1)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
	if(UNIX)
		set(CMAKE_COMPILER_IS_UNIX_ICC 1)
	else()
		# CMAKE_COMPILER_IS_WIN_ICC 的 c++11 模式命令行参数不一样
		set(CMAKE_COMPILER_IS_WIN_ICC 1)
	endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX OR CMAKE_COMPILER_IS_UNIX_ICC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()


add_subdirectory(videomgr)

add_subdirectory(gui/qt5)
