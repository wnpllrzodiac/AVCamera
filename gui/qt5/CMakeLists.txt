set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

cmake_policy(SET CMP0020 NEW)
add_definitions(-DQT_NO_KEYWORDS)

find_package(Qt5Widgets REQUIRED)

file(GLOB HDRS include/*.h*)
file(GLOB SRCS *.cpp *.h*)
file(GLOB UIS *.ui)

qt5_wrap_ui(UI_SRCS ${UIS})
qt5_add_resources(RC_SRCS avcamera.qrc)

set(SRCS ${SRCS} ${UI_SRCS} ${RC_SRCS})

add_executable(avcamera WIN32 MACOSX_BUNDLE ${SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/../win32/AVCamera.rc)
target_link_libraries(avcamera Qt5::Widgets videomgr)
