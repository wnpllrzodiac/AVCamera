
#pragma once


#include "h264writer.hpp"
#include "filter.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/signals2/signal.hpp>
#include <boost/thread/thread.hpp> 


namespace VideoMgr
{
	enum CameraSatus
	{
		CREATED   = 0,
		RECORDING = 1,
		PAUSED    = 2,
		STOPPED   = 4,
		EXITED    = 8,
	};
	class Camera
	{
	public:
		Camera();
		~Camera();
		
		int start(std::string file, int width, int height, int bit_rate); //start recording
		int pause(); //pause recording
		int stop();  //stop recording
		int exit();
		void get_curr_frame(cv::Mat& frame);
		cv::Mat                           last_frame;
	private:
		void thread_task();	//do not use
	public:
		boost::signals2::signal<void ()>   refresh_sign;
		CameraSatus                        _status;
	private:
		boost::thread                      _camera_thread;
		cv::VideoCapture                   _video;
		std::shared_ptr<H264Writer>        _h264;
		std::shared_ptr<Filter>            _filter;
		CameraSatus                        _last_status;
		std::string                        _file;
	};

}
