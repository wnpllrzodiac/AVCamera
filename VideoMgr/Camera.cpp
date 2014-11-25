
#include "stdafx.h"
#include "Camera.h"
#include "util.h"
#include "H264Writer.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

namespace VideoMgr
{

	Camera::Camera()
		: _status(CREATED)
	{
		ffmpeg_init_all();
		
	}

	Camera::~Camera()
	{
		delete _video;
	}


	void Camera::thread_task( String^ file, int width, int height, int bit_rate )
	{
		H264Writer h264;
		h264.create(file, width, height, bit_rate);

		_video = new cv::VideoCapture();
		_video->open(0);
		cv::Mat frame;
		while (true)
		{
			if( _status == PAUSED ) 
			{
				cv::waitKey(100); continue;
			}
			else if( _status == STOPPED ) break;
			else if( _status == RECORDING )
			{
				*_video >> frame;
				cv::imshow("video", frame);
				h264 << frame;
				cv::waitKey(100);
			}
		}
		cv::destroyWindow("video");
		h264.close();
	}


	int Camera::start()
	{
		if(_status == CREATED)
		{
			_status = RECORDING;
		}
		else if(_status == PAUSED)
		{
			_status = RECORDING;
		}
		else if(_status == STOPPED)
		{
			_status = RECORDING;
		}
		return safe_cast<int>(_status);
	}

	int Camera::pause()
	{
		if(!_video->isOpened()) return 0;
		if(_status == RECORDING)
		{
			_status = PAUSED;
		}
		return safe_cast<int>(_status);
	}

	int Camera::stop()
	{
		if(!_video->isOpened()) return 0;
		if(_status == CREATED)
		{
			_status = STOPPED;
		}
		else if(_status == RECORDING)
		{
			_status = STOPPED;
		}
		else if(_status == PAUSED)
		{
			_status = STOPPED;
		}
		return safe_cast<int>(_status);
	}

}
