// VideoMgr.h

#pragma once
#include "stdafx.h"

#include "opencv2\core\core.hpp"

using namespace System;

namespace VideoMgr {

	public ref class H264Writer
	{
	public:
		H264Writer( int width, int height, int bit_rate );
		inline bool operator << ( cv::Mat& mat );

	private:
		int width;
		int height;
		int bit_rate;
	};
}
