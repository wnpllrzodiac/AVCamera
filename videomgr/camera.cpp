
#include "stdafx.h"
#include "camera.h"
#include "h264writer.h"
#include "filter.h"
#include "util.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace VideoMgr
{

	Camera::Camera()
		: _status(CREATED)
	{
		av_register_all();

		_video.open(0);
	}

	Camera::~Camera()
	{
		_video.release();
		_h264.reset();
		_filter.reset();
	}


	bool Camera::thread_task()
	{
		cv::Mat frame;
		unsigned long timer, curr_time;
		timer = curr_time = get_now_time();
		unsigned long duration = 0;
		while (true)
		{
			if( _status == CREATED)
			{
				timer = curr_time = get_now_time();
				custom_sleep(100); continue;
			}
			else if(_status == PAUSED)
			{
				curr_time = get_now_time();
				cv::waitKey(100); continue;
			}
			else if(_status == STOPPED)
			{
				if(_last_status == RECORDING
					|| _last_status == PAUSED)
				{
					cv::destroyWindow("video");
					_h264->close();
					_h264.reset();
					_filter.reset();
					_last_status = _status;
				}
				timer = curr_time = get_now_time();
				custom_sleep(100); continue;
			}
			else if(_status == RECORDING)
			{
				_video >> frame;
				// Filter
				bool isEncode = true;
				isEncode = isEncode && _filter->show_datetime(frame) && _filter->give_up_frame(frame, 15);
				//

				curr_time = get_now_time();
				cv::imshow("video", frame);
				unsigned long dur = curr_time - timer;
				if(isEncode)
				{
					duration += dur;
					_h264->write(frame, duration > 40 ? duration : 40);
					cv::waitKey(duration > 40 ? 1 : (40 - duration));
					duration = 0;
				}
				else
				{
					duration += dur;
					cv::waitKey( 1 );
				}

				timer = curr_time;

			}
			else if(_status == EXITED)
			{
				if(_last_status == STOPPED
					|| _last_status == CREATED)
				{
					cv::destroyWindow("video");
				}
				break;
			}

		}

		return true;
	}


	int Camera::start( std::string file, int width, int height, int bit_rate )
	{
		//create h264writer
		_h264.reset(new H264Writer());
		if(!_h264->create(file, width, height, bit_rate))
			return static_cast<int>(CREATED);

		//create filter
		_filter.reset(new Filter(width, height));

		_last_status = _status;
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
		return static_cast<int>(_status);
	}

	int Camera::pause()
	{
		if(!_video.isOpened()) return 0;
		_last_status = _status;
		if(_status == RECORDING)
		{
			_status = PAUSED;
		}
		return static_cast<int>(_status);
	}

	int Camera::stop()
	{
		if(!_video.isOpened()) return 0;
		_last_status = _status;
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
		return static_cast<int>(_status);
	}
	int Camera::exit()
	{
		_last_status = _status;
		_status = EXITED;
		if(_h264 != nullptr) _h264->close();
		return static_cast<int>(_status);
	}
}
