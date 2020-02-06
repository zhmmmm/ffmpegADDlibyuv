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
	//4.�ҵ�������
	AVCodec *codec = FF::GetInstance()->findDecoder(code->codec_id);
	AVFrame *yuvFrame = av_frame_alloc();
	FF::GetInstance()->openDecoder(code, codec);


	int go_frame = 0;
	FILE *pf = nullptr;
	// y  �ڰ�
	// u v ����ɫ��
	pf = fopen("D:\\test.yuv", "wb");
	int count = 0;

	AVPacket *pk = (AVPacket*)av_malloc(sizeof(AVPacket));
	AVFrame *frame = av_frame_alloc();
	AVFrame *rgb_frame = av_frame_alloc();
	int width = code->width; int height = code->height;
	int y_size, u_size, v_size;
	struct SwsContext *img_convert_ctx;
	//����ת��������
	img_convert_ctx = sws_getContext(code->width, code->height, code->pix_fmt,
		code->width, code->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	while (FF::GetInstance()->readFrame(pk) >= 0)
	{
		if (pk->stream_index == video_index)
		{
			//����֡�ŵ�frame
			FF::GetInstance()->get_video_FrameData(code, frame, &go_frame, pk);

			//����ɹ�
			if (go_frame)
			{
				//avpicture_fill((AVPicture*)rgb_frame, buf, AV_PIX_FMT_RGBA, width, height);
				//תRGB
				//libyuv::I420ToARGB(
				//	frame->data[0],frame->linesize[0],
				//	frame->data[2], frame->linesize[2],
				//	frame->data[1], frame->linesize[1],
				//	rgb_frame->data[0], rgb_frame->linesize[0],
				//	width,height);

								//�ɹ�����һ֡
				sws_scale(img_convert_ctx, (const unsigned char* const*)frame->data, frame->linesize,
					0,
					code->height,
					rgb_frame->data, rgb_frame->linesize);//ת��ͼ���ʽ

				y_size = width * height;
				u_size = y_size / 4;
				v_size = y_size / 4;
				//����->Y����
				fwrite(frame->data[0], 1, y_size, pf);
				//���->U����
				fwrite(frame->data[1], 1, u_size, pf);
				//�����->V����
				fwrite(frame->data[2], 1, v_size, pf);

				printf(" ���� �� %d ֡ width = %d, height = %d \n", count++,frame->width,frame->height);
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
	//��
	bool open(string fileOrWeb);
	//��ȡ��������
	int getAudioIndex(AVFormatContext *AVFC);
	//��ȡ��������
	int getVideoIndex(AVFormatContext *AVFC);
	//���������õ�������
	AVCodecContext *getCodecContext(int index);
	AVCodecContext *getCodecContext(AVFormatContext *AVFC, int index);
	//���ҽ�����
	AVCodec *findDecoder(AVCodecID ID);
	//�򿪽�����
	void openDecoder(AVCodecContext *codecContext,AVCodec *codec);
	//loop ��֡,����֡position
	int readFrame(AVPacket *outPacket);
	//loop read framedata
	void get_video_FrameData(AVCodecContext *InCodecContext,AVFrame *OutFrame,int *OutPos,AVPacket *InPacket);

public:
	void free();
private:
	FF();
	~FF();
};