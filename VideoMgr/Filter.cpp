
#include "Stdafx.h"
#include "Filter.h"
#include "util.h"

#include <opencv2\core\core.hpp>

using namespace System;
using namespace System::Threading;

#using < mscorlib.dll >
#using < System.Windows.Forms.dll >

namespace VideoMgr
{
	Filter::Filter(int width, int height)
		:_width(width)
		,_height(height)
	{
		last_frame.create(height, width, CV_8UC3);
	}

	Filter::~Filter()
	{
		last_frame.~Mat();
	}

	bool Filter::show_datetime( cv::Mat& frame )
	{
		String^ datetimestr =  DateTime::Now.ToString("yyyy-MM-dd HH:mm:ss");
		std::string datetimestr2;
		String2string(datetimestr, datetimestr2);
		cv::putText(frame, datetimestr2, cv::Point(2,_height-2), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0,255,0));
		return true;
	}

	bool Filter::give_up_frame( cv::Mat& frame )
	{

		return true;
	}

	bool Filter::blur( cv::Mat& frame )
	{
		return true;
	}

}