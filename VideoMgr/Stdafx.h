// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#include <string>
#include <vector>

//opencv library
#ifdef _DEBUG
#pragma comment (lib, "opencv_core249d.lib")
#pragma comment (lib, "opencv_imgproc249d.lib")
//#pragma comment (lib, "opencv_highgui249d.lib")
#pragma comment (lib, "opencv_objdetect249d.lib")
//#pragma comment (lib, "opencv_video249d.lib")
#endif

#ifdef NDEBUG
#pragma comment (lib, "opencv_core249.lib")
#pragma comment (lib, "opencv_imgproc249.lib")
#pragma comment (lib, "opencv_highgui249.lib")
#pragma comment (lib, "opencv_objdetect249.lib")
//#pragma comment (lib, "opencv_video249.lib")
#endif

//ffmpeg library
#pragma comment (lib, "avcodec.lib")
#pragma comment (lib, "avformat.lib")
#pragma comment (lib, "avutil.lib")
//#pragma comment (lib, "swscale.lib")
//#pragma comment (lib, "swresample.lib")