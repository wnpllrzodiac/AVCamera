
#pragma once

#include "stdafx.h"

#include <opencv2\core\core.hpp>

namespace VideoMgr
{
	class Filter
	{
	public:
		Filter(){};
		Filter(int width, int height);
		~Filter();
		// function return value : true: draw , false: jump
		bool show_datetime(cv::Mat& frame);
		bool give_up_frame(cv::Mat& frame);
		bool blur(cv::Mat& frame);

	private:
		cv::Mat last_frame;
		int _width;
		int _height;
	};

}
