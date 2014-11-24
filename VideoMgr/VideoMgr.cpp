// 这是主 DLL 文件。

#include "stdafx.h"
#include "VideoMgr.h"
#include "util.h"

#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

extern "C"
{
#include <libavutil\opt.h>
#include <libavcodec\avcodec.h>
#include <libavutil\channel_layout.h>
#include <libavutil\imgutils.h>
#include <libavutil\mathematics.h>
#include <libavutil\samplefmt.h>
};


namespace VideoMgr
{
	H264Writer::H264Writer()
	{
	}

	bool H264Writer::create( String^ file, int width, int height, int bit_rate )
	{
		std::string filename;
		String2string(file, filename);
		AVOutputFormat* output_format = av_guess_format(nullptr, filename.c_str(), nullptr);
		if(output_format == nullptr)
		{
			printf("could not deduce output format from outfile extension\n");
			return false;
		}

		AVFormatContext* format_context = avformat_alloc_context();
		if(format_context == nullptr)
		{
			printf("Memory error\n");
			return false;
		}
		format_context->oformat = output_format;
		strcpy_s(format_context->filename, 1024, filename.c_str());

		// create video stream and video encoder
		AVCodec* video_codec = avcodec_find_encoder(output_format->video_codec);
		if (video_codec == nullptr)
		{
			printf("codec not found\n");
			return false;
		}
		AVStream* video_stream = avformat_new_stream(format_context, video_codec);
		if(video_stream == nullptr)
		{
			printf("could not alloc video stream\n");
			return false;
		}
		avcodec_get_context_defaults3(video_stream->codec, video_codec);

		AVCodecContext* video_codec_context = video_stream->codec;
		video_codec_context->codec_id       = AV_CODEC_ID_H264;
		video_codec_context->pix_fmt        = AV_PIX_FMT_YUV420P;
		video_codec_context->codec_type     = AVMEDIA_TYPE_VIDEO;
		video_codec_context->bit_rate       = 1815484;
		video_codec_context->width          = width;
		video_codec_context->height         = height;
		video_codec_context->time_base.num  = 1001;
		video_codec_context->time_base.den  = 30000;
		video_codec_context->gop_size       = 12;
		video_codec_context->max_b_frames   = 0;
		if (format_context->oformat->flags & AVFMT_GLOBALHEADER)
			video_codec_context->flags |= CODEC_FLAG_GLOBAL_HEADER;

		av_opt_set(video_codec_context->priv_data, "tune", "zerolatency", 0);
		av_opt_set(video_codec_context->priv_data, "preset", "superfast", 0);

		if(avcodec_open2(video_codec_context, video_codec, nullptr) < 0)
		{
			printf("can't open the output video codec\n");
			return false;
		}

		format_context->video_codec_id = output_format->video_codec;

		// open output file to write
		if ((format_context->flags & AVFMT_NOFILE) == 0)
		{
			if(avio_open(&format_context->pb, filename.c_str(), AVIO_FLAG_READ_WRITE) < 0)
			{
				printf("can't open the output file : %s.", filename.c_str());
				return false;
			}
		}

		_video_pts = 0;

		//return the result
		_filename = file;
		_width    = width;
		_height   = height;

		_format   = output_format;
		_context  = format_context;
		_vstream  = video_stream;
		_vcodec   = video_codec_context;

		write_header();

		return true;
	}

	bool H264Writer::operator<<( cv::Mat& mat )
	{
		if (mat.rows != _height || mat.cols != _width)
		{
			//resize image
			cv::resize(mat, mat, cv::Size(_width, _height));
		}

		return false;
	}

	void H264Writer::close()
	{
		write_trailer();

		//interior_ptr<AVFormatContext*> p = &(_context);
		pin_ptr<AVFormatContext*> pinptr =  (&_context);
		avformat_close_input(pinptr);
		_format  = nullptr;
		_vstream = nullptr;
		_vcodec  = nullptr;
	}

	bool H264Writer::write_header()
	{
		int error = avformat_write_header(_context, NULL);
		if (error < 0)
		{
			printf("Could not write output file header.");
			return false;
		}
		return true;
	}

	bool H264Writer::write_trailer()
	{
		int error = av_write_trailer(_context);
		if (error < 0)
		{
			printf("Could not write output file trailer.");
			return false;
		}
		return true;
	}

	

}
