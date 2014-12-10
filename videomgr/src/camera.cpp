
#include "camera.hpp"
#include "h264writer.hpp"
#include "filter.hpp"
#include "util.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/signals2/signal.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

namespace VideoMgr
{
	Camera::Camera()
		: _status(CREATED)
		, _camera_thread(boost::bind(&Camera::thread_task, this))
	{
		av_register_all();

		_video.open(0);
	}

	Camera::~Camera()
	{
		_camera_thread.interrupt();
		_camera_thread.join();
		_video.release();
		_h264.reset();
		_filter.reset();
	}


	void Camera::thread_task()
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
				boost::this_thread::sleep(boost::posix_time::microseconds(800)); continue;
			}
			else if(_status == PAUSED)
			{
				curr_time = get_now_time();
				boost::this_thread::sleep(boost::posix_time::microseconds(800)); continue;
			}
			else if(_status == STOPPED)
			{
				if(_last_status == RECORDING
					|| _last_status == PAUSED)
				{
					_h264->close();
					_h264.reset();
					_filter.reset();
					_last_status = _status;
				}
				timer = curr_time = get_now_time();
				boost::this_thread::sleep(boost::posix_time::microseconds(800)); continue;
			}
			else if(_status == RECORDING)
			{
				_video >> frame;
				// Filter
				bool isEncode = true;
				isEncode = isEncode && _filter->give_up_frame(frame, 5);
				isEncode = isEncode && _filter->show_datetime(frame);
				//

				curr_time = get_now_time();
				//cv::imshow("video", frame); for test
				unsigned long dur = curr_time - timer;
				if(isEncode)
				{
					duration += dur;
					_h264->write(frame, duration > 40 ? duration : 40);
					if(duration < 40) boost::this_thread::sleep(boost::posix_time::microseconds(40 - duration));
					//cv::waitKey(duration > 40 ? 1 : (40 - duration));
					duration = 0;
				}
				else
				{
					duration += dur;
					//boost::this_thread::sleep(boost::posix_time::microseconds(10));
				}

				//send sign
				refresh_sign();

				timer = curr_time;
				frame.copyTo(last_frame);//backup to last_frame everyone in recording
			}
			else if(_status == EXITED)
			{
				if(_last_status == STOPPED
					|| _last_status == CREATED)
				{
					;//cv::destroyWindow("video");//for test
				}
				break;
			}

		}
	}


	int Camera::start( std::string file, int width, int height, int bit_rate )
	{
		//create h264writer
		_h264.reset(new H264Writer());
		if(!_h264->create(file, width, height, bit_rate))
			return static_cast<int>(CREATED);

		//create filter
		_filter.reset(new Filter(width, height));

		//create last frame mat
		last_frame.create(height, width, CV_8UC3);

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
		last_frame.release();
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
		stop();
		boost::this_thread::sleep(boost::posix_time::microseconds(800));
		_status = EXITED;
		return static_cast<int>(_status);
	}

	void Camera::get_curr_frame( cv::Mat& frame )
	{
		frame = last_frame;//it is not copy memory
	}

}
