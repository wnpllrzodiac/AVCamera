// 这是主 DLL 文件。

#include "stdafx.h"
#include "H264Writer.h"
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

		//init output video frame
		_output_video_frame = av_frame_alloc();
		int length = avpicture_get_size(AV_PIX_FMT_YUV420P, width, height);
		uint8_t* buffer=(uint8_t *)av_malloc(length * sizeof(uint8_t));
		avpicture_fill((AVPicture*)_output_video_frame, buffer, AV_PIX_FMT_YUV420P, width, height);

		_rgb_scaler = sws_getContext(width, height, AV_PIX_FMT_BGR24, width, height, AV_PIX_FMT_YUV420P, 0, nullptr, nullptr, nullptr);

		//init pts
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
		write(mat, 33);

		return true;
	}

	bool H264Writer::write( cv::Mat& mat, int64 duration )
	{
		if (mat.rows != _height || mat.cols != _width)
		{
			//resize image width and height to frame size
			cv::resize(mat, mat, cv::Size(_width, _height));
		}
		//frame convert from BGR to YUV420
		//cvtColor(mat, mat, CV_BGR2YUV_I420);
		uint8_t* data[] = { mat.data };
		int src_linesize[] = { _width * 3 };
		int h = sws_scale(_rgb_scaler, data, src_linesize, 0, _height, _output_video_frame->data, _output_video_frame->linesize);

		//memcpy( _output_video_frame->data, mat.data, mat.rows * mat.cols * mat.channels() );
		_output_video_frame->pts = _video_pts * _vcodec->time_base.num / _vcodec->time_base.den;
		_output_video_frame->pict_type = AV_PICTURE_TYPE_S;
		_output_video_frame->format = AV_PIX_FMT_YUV420P;
		_output_video_frame->width = _width;
		_output_video_frame->height = _height;

		int frame_finished;
		AVPacket output_packet = {0};
		av_init_packet(&output_packet);
		int ret = avcodec_encode_video2(_vcodec, &output_packet, _output_video_frame, &frame_finished);
		if ( ret == 0 && frame_finished )
		{
			output_packet.stream_index = _vstream->index;
			if (output_packet.pts != AV_NOPTS_VALUE)
			{
				output_packet.pts = av_rescale_q(output_packet.pts, _vcodec->time_base, _vstream->time_base);
			}
			if (output_packet.dts != AV_NOPTS_VALUE)
			{
				output_packet.dts = av_rescale_q(output_packet.dts, _vcodec->time_base, _vstream->time_base);
			}
			if(av_interleaved_write_frame(_context, &output_packet) < 0)
			{
				printf("Fail to write the video frame #%d.\n", _video_pts);
				av_free_packet(&output_packet);
				return false;
			}
		}
		else
		{
			printf("avcodec_encode_video2 failed\n");
		}
		av_free_packet(&output_packet);
		_video_pts += duration;

		return true;
	}

	void H264Writer::close()
	{
		write_trailer();

		//interior_ptr<AVFormatContext*> p = &(_context);
		pin_ptr<AVFormatContext*> AVFormatContextPtr =  (&_context);
		avformat_close_input(AVFormatContextPtr);
		sws_freeContext(_rgb_scaler);
		pin_ptr<AVFrame*> AVFramePtr =  (&_output_video_frame);
		av_frame_free(AVFramePtr);
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
