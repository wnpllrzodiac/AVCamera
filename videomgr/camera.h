
#pragma once

#include <memory>
#include <string>

#include "h264writer.h"
#include "filter.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


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

		bool thread_task();
		int start(std::string file, int width, int height, int bit_rate); //start recording
		int pause(); //pause recording
		int stop();  //stop recording
		int exit();

	private:
		cv::VideoCapture                   _video;
		std::shared_ptr<H264Writer>        _h264;
		std::shared_ptr<Filter>            _filter;
		CameraSatus                        _status;
		CameraSatus                        _last_status;
		std::string                        _file;
	};

}
