#ifndef __FF_H__
#define __FF_H__

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavfilter/avfilter.h"
#include "libpostproc/postprocess.h"
#include "libavutil/avutil.h"
}
#include "libyuv/libyuv.h"
#define FFMPEGFORWINDOWS 0

#ifdef FFMPEGFORWINDOWS
	#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER) || defined(_WINDOWS) || defined(WINDOWS)
		#pragma comment(lib,"avcodec.lib")
		#pragma comment(lib,"avdevice.lib")
		#pragma comment(lib,"avfilter.lib")
		#pragma comment(lib,"avformat.lib")
		#pragma comment(lib,"avutil.lib")
		#pragma comment(lib,"postproc.lib")
		#pragma comment(lib,"swresample.lib")
		#pragma comment(lib,"swscale.lib")
	#endif
#endif
#endif

#include <string>
using namespace std;

#define FFS FF::GetInstance()

/*

	AVFormatContext *pContext = nullptr;
	if (FF::GetInstance()->open("D:\\Material\\Po.flv"))
	{
		pContext = FF::GetInstance()->getFormatContext();
	}

	int video_index = FF::GetInstance()->getVideoIndex(pContext);
	int audio_index = FF::GetInstance()->getAudioIndex(pContext);

	AVCodecContext *code = FF::GetInstance()->getCodecContext(video_index);
	//4.找到解码器
	AVCodec *codec = FF::GetInstance()->findDecoder(code->codec_id);
	AVFrame *yuvFrame = av_frame_alloc();
	FF::GetInstance()->openDecoder(code, codec);


	int go_frame = 0;
	FILE *pf = nullptr;
	// y  黑白
	// u v 像素色度
	pf = fopen("D:\\test.yuv", "wb");
	int count = 0;

	AVPacket *pk = (AVPacket*)av_malloc(sizeof(AVPacket));
	AVFrame *frame = av_frame_alloc();
	AVFrame *rgb_frame = av_frame_alloc();
	int width = code->width; int height = code->height;
	int y_size, u_size, v_size;
	struct SwsContext *img_convert_ctx;
	//申请转换上下文
	img_convert_ctx = sws_getContext(code->width, code->height, code->pix_fmt,
		code->width, code->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	while (FF::GetInstance()->readFrame(pk) >= 0)
	{
		if (pk->stream_index == video_index)
		{
			//数据帧放到frame
			FF::GetInstance()->get_video_FrameData(code, frame, &go_frame, pk);

			//解码成功
			if (go_frame)
			{
				//avpicture_fill((AVPicture*)rgb_frame, buf, AV_PIX_FMT_RGBA, width, height);
				//转RGB
				//libyuv::I420ToARGB(
				//	frame->data[0],frame->linesize[0],
				//	frame->data[2], frame->linesize[2],
				//	frame->data[1], frame->linesize[1],
				//	rgb_frame->data[0], rgb_frame->linesize[0],
				//	width,height);

								//成功解码一帧
				sws_scale(img_convert_ctx, (const unsigned char* const*)frame->data, frame->linesize,
					0,
					code->height,
					rgb_frame->data, rgb_frame->linesize);//转换图像格式

				y_size = width * height;
				u_size = y_size / 4;
				v_size = y_size / 4;
				//首先->Y数据
				fwrite(frame->data[0], 1, y_size, pf);
				//其次->U数据
				fwrite(frame->data[1], 1, u_size, pf);
				//再其次->V数据
				fwrite(frame->data[2], 1, v_size, pf);

				printf(" 解码 第 %d 帧 width = %d, height = %d \n", count++,frame->width,frame->height);
				int var = 0;
			}
			av_free_packet(pk);
		}
	}
	if (pf)
	{
		fclose(pf);
		pf = nullptr;
	}
	av_frame_free(&frame);
	avcodec_close(code);
	avformat_free_context(pContext);
	printf("ok\n");
	_sleep(3000);

*/

class FF
{
	AVFormatContext *m_FormatContext = nullptr;
public:
	static FF *GetInstance();
	AVFormatContext *getFormatContext();
	//打开
	bool open(string fileOrWeb);
	//获取类型索引
	int getAudioIndex(AVFormatContext *AVFC);
	//获取类型索引
	int getVideoIndex(AVFormatContext *AVFC);
	//根据索引得到解码器
	AVCodecContext *getCodecContext(int index);
	AVCodecContext *getCodecContext(AVFormatContext *AVFC, int index);
	//查找解码器
	AVCodec *findDecoder(AVCodecID ID);
	//打开解码器
	void openDecoder(AVCodecContext *codecContext,AVCodec *codec);
	//loop 读帧,返回帧position
	int readFrame(AVPacket *outPacket);
	//loop read framedata
	void get_video_FrameData(AVCodecContext *InCodecContext,AVFrame *OutFrame,int *OutPos,AVPacket *InPacket);

public:
	void free();
private:
	FF();
	~FF();
};
