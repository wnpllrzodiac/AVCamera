// VideoMgr.h

#pragma once
#include "stdafx.h"

#include <opencv2\core\core.hpp>

extern "C"
{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libswscale\swscale.h>
}


namespace VideoMgr {

	class H264Writer
	{
	public:
		H264Writer();
		~H264Writer();
		bool create( std::string file, int width, int height, int bit_rate );
		bool operator << ( cv::Mat& mat );
		bool write( cv::Mat& mat, int64 duration );
		void close();

	private:
		bool write_header();
		bool write_trailer();

	private:
		std::string      _filename;
		int              _width;
		int              _height;
		int              _bit_rate;

		int64            _video_pts;

		AVFormatContext* _context;
		AVOutputFormat*  _format;
		AVStream*        _vstream;
		AVCodecContext*  _vcodec;

		SwsContext*      _rgb_scaler;

		AVFrame*         _output_video_frame;
	};

}
