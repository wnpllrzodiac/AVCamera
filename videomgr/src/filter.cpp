
#include "filter.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <time.h>
#include <stdio.h>


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
		time_t tt = time(NULL);
		tm* t= localtime(&tt);
		char timestr[30];
		sprintf(timestr, "%d-%02d-%02d %02d:%02d:%02d",
			t->tm_year + 1900,
			t->tm_mon + 1,
			t->tm_mday,
			t->tm_hour,
			t->tm_min,
			t->tm_sec);
		cv::putText(frame, std::string(timestr), cv::Point(2,_height-2), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0,255,0));
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
