// 这是主 DLL 文件。

#include "stdafx.h"
#include "VideoMgr.h"

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
	H264Writer::H264Writer( int width, int height, int bit_rate )
		: _width(width)
		, _height(height)
		, _bit_rate(bit_rate)
		, _avcodec(nullptr)
		, _avcontext(nullptr)
	{

		_avcodec = avcodec_find_encoder(AV_CODEC_ID_H264);
		if (!_avcodec) {
			fprintf(stderr, "Codec not found\n");
			exit(1);
		}
		_avcontext = avcodec_alloc_context3(_avcodec);
		if (!_avcontext) {
			fprintf(stderr, "Could not allocate video codec context\n");
			exit(1);
		}
		///* put sample parameters */
		//_avcontext->bit_rate = 400000;
		///* resolution must be a multiple of two */
		//_avcontext->width = 352;
		//_avcontext->height = 288;
		///* frames per second */
		//AVRational avRational;
		//avRational.num = 1;
		//avRational.den = 25;
		//_avcontext->time_base = avRational;
		//_avcontext->gop_size = 10;
		//_avcontext->max_b_frames = 1;
		//_avcontext->pix_fmt = AV_PIX_FMT_YUV420P;

		////av_opt_set(_avcontext->priv_data, "preset", "slow", 0);
		//av_opt_set(_avcontext->priv_data, "preset", "superfast", 0);
		//av_opt_set(_avcontext->priv_data, "tune", "zerolatency", 0);

		///* open it */
		//if (avcodec_open2(_avcontext, _avcodec, NULL) < 0) {
		//	fprintf(stderr, "Could not open codec\n");
		//	exit(1);
		//}
		//
		//AVFrame* frame = av_frame_alloc();
		//if (!frame) {
		//	fprintf(stderr, "Could not allocate video frame\n");
		//	exit(1);
		//}
		//frame->format = _avcontext->pix_fmt;
		//frame->width = _avcontext->width;
		//frame->height = _avcontext->height;

		//int got_output = 0;
		//AVPacket pkt;
		//int ret = avcodec_encode_video2(_avcontext, &pkt, frame, &got_output);
		//if (ret < 0) {
		//	fprintf(stderr, "Error encoding frame\n");
		//	exit(1);
		//}
		//if (got_output) {
		//	av_free_packet(&pkt);
		//}
		//
		//avcodec_close(_avcontext);
		//av_free(_avcontext);
		//av_freep(&frame->data[0]);
		//av_frame_free(&frame);
		//printf("\n");
		//
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
		
}
