
find_package(OpenCV REQUIRED)

add_library(videomgr STATIC src/camera.cpp src/filter.cpp src/h264writer.cpp)

target_include_directories(videomgr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(videomgr PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(videomgr ${OpenCV_LIBS})
