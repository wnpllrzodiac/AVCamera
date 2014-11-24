
#pragma once
using namespace System;

#include "stdafx.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

namespace VideoMgr
{
	enum CameraSatus
	{
		CREATED   = 0,
		RECORDING = 1,
		PAUSED    = 2,
		STOPPED      = 4,
	};
	public ref class Camera
	{
	public:
		Camera( String^ file );
		~Camera();

		int start(); //start recording
		int pause(); //pause recording
		int stop();  //stop recording

	private:
		cv::VideoCapture* _video;
		CameraSatus       _status;
		String^           _file;
	};

}
