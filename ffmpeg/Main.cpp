#include <iostream>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}
#include "libyuv/libyuv.h"
#define FFMPEGFORWINDOWS
#include "FF.h"


int main()
{
	av_register_all();

	AVFormatContext *s = nullptr;
	s = avformat_alloc_context();
	//https://v.youku.com/v_show/id_XMTI2NjUzMTY4MA==.html?spm=a2hcb.collection.page.21
	avformat_open_input(&s,"D:\\Material\\Po.flv",nullptr,nullptr);
	if (s == nullptr) { return -1; }
	avformat_find_stream_info(s, nullptr);

	int index = 1;
	for (int i = 0; i < s->nb_streams; i++)
	{
		if (s->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			index = i;
			break;
		}
	}

	AVCodecContext *code = s->streams[index]->codec;
	AVCodec *codec = avcodec_find_decoder(code->codec_id);
	AVFrame *yuvFrame = av_frame_alloc();
	int ret = avcodec_open2(code, codec, nullptr);
	if (ret < 0)
	{
		avcodec_close(code);
		avformat_free_context(s);
		return -1;
	}

	uint8_t *out_buf = (uint8_t*)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P,
		code->width,code->height));
	avpicture_fill((AVPicture*)yuvFrame, out_buf, AV_PIX_FMT_YUV420P, 
		code->width, code->height);

	AVPacket *pk = (AVPacket*)av_malloc(sizeof(AVPacket));
	AVFrame *frame = av_frame_alloc();
	
	//设置像素格式
	//avpicture_fill((AVPicture*)yuvFrame,,AV_PIX_FMT_YUV420P,
	//	);
	struct SwsContext *sws_ctx = sws_getContext(
		code->width, code->height, code->pix_fmt,
		code->width, code->height,AV_PIX_FMT_YUV420P,
		SWS_BILINEAR,nullptr,nullptr,nullptr);

	int go_frame = 0;
	FILE *pf = nullptr;
	// y  黑白
	// u v 像素色度
	pf = fopen("test.yuv", "wb");
	int count = 0;
	while (av_read_frame(s,pk) >= 0)
	{
		if (pk->stream_index == index)
		{
			avcodec_decode_video2(code, frame, &go_frame, pk);
			//解码成功
			if (go_frame)
			{

				//sws_scale();
				sws_scale(sws_ctx,
					frame->data, frame->linesize, 0, frame->height,
					yuvFrame->data, yuvFrame->linesize);

				int y_size = code->width * code->height;
				fwrite(yuvFrame->data[0], 1, y_size, pf);
				fwrite(yuvFrame->data[1], 1, y_size / 4, pf);
				fwrite(yuvFrame->data[2], 1, y_size / 4, pf);
				printf(" 解码 %d帧\n", count++);

				//--> yuv to rgb
				//google libyuv

				

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
	avformat_free_context(s);
	return 0;
}







