
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

find_library(AVUTIL_LIBRARY NAMES avutil)
find_library(AVFORMAT_LIBRARY NAMES avformat)
find_library(AVCODEC_LIBRARY NAMES avcodec)
find_library(AVFILTER_LIBRARY NAMES avfilter)
find_library(SWSCALE_LIBRARY NAMES swscale)

add_library(videomgr STATIC src/camera.cpp src/filter.cpp src/h264writer.cpp)

target_include_directories(videomgr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(videomgr PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(videomgr ${OpenCV_LIBS})
target_link_libraries(videomgr ${AVUTIL_LIBRARY} ${AVFORMAT_LIBRARY} ${AVCODEC_LIBRARY} ${AVFILTER_LIBRARY} ${SWSCALE_LIBRARY})
target_link_libraries(videomgr ${CMAKE_THREAD_LIBS_INIT})
