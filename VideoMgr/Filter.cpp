
#include "Stdafx.h"
#include "Filter.h"
#include "util.h"

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

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

	bool Filter::give_up_frame( cv::Mat& frame, double diffVal )
	{
		cv::Mat cha_mat;
		cv::absdiff(frame, last_frame, cha_mat);
		cv::cvtColor(cha_mat, cha_mat, CV_BGR2GRAY);
		cv::threshold(cha_mat, cha_mat, 5 , 255, CV_THRESH_BINARY_INV);
		double percent = 1.0;
		percent = percent - countNonZero(cha_mat) / (double)(cha_mat.cols * cha_mat.rows);
		percent *= 100;
		char str[20];
		sprintf(str, "Diff: %.2f%%", percent);
		cv::putText(frame, std::string(str), cv::Point(2,_height-100), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0,0,255));
		if(percent > diffVal)
		{
			frame.copyTo(last_frame);
			return true;
		}
		else
		{
			return false;
		}
		
	}

	bool Filter::blur( cv::Mat& frame )
	{
		return true;
	}

}