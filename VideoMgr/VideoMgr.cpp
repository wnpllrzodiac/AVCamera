// 这是主 DLL 文件。

#include "stdafx.h"
#include "VideoMgr.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"

namespace VideoMgr
{
	H264Writer::H264Writer( int width, int height, int bit_rate )
	{
		
		
	}

	bool H264Writer::operator<<( cv::Mat& mat )
	{
		if (mat.rows != _height || mat.cols != _width)
		{
			//resize image
			resize(mat, mat, Size(_width, _height));
		}

		
	}

}
