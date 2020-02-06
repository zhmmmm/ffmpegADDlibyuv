#include "FFMPEG.h"

FF *FF::GetInstance()
{
	static FF obj;
	return &obj;
}

FF::FF()
{
	av_register_all();
	this->m_FormatContext = nullptr;
}

FF::~FF()
{
	this->free();
}

void FF::free()
{
	if (this->m_FormatContext != nullptr)
	{
		avformat_free_context(this->m_FormatContext);
		this->m_FormatContext = nullptr;
	}
}

AVFormatContext *FF::getFormatContext()
{
	return this->m_FormatContext;
}

bool FF::open(string fileOrWeb)
{
	if (fileOrWeb.length() > 0)
	{
		this->m_FormatContext = avformat_alloc_context();

		if (this->m_FormatContext != nullptr)
		{

			int ret = avformat_open_input(&this->m_FormatContext,
				fileOrWeb.c_str(),
				nullptr, nullptr);

			if (ret >= 0)
			{
				ret = avformat_find_stream_info(this->m_FormatContext, nullptr);

				if (ret >= 0)
				{
					return true;
				}
			}
		}
		printf("open fail\n");
	}
	printf("open fail\n");
	return false;
}

AVCodec *FF::findDecoder(AVCodecID ID)
{
	AVCodec *ret = avcodec_find_decoder(ID);
	return ret;
}

void FF::openDecoder(AVCodecContext *codecContext, AVCodec *codec)
{
	int ret = avcodec_open2(codecContext, codec, nullptr);
	if (ret >= 0)
	{
		printf("open decoder success !\n");
		return;
	}
	printf("open decoder fail !\n");
}

int FF::readFrame(AVPacket *outPacket)
{
	if (this->m_FormatContext != nullptr && outPacket != nullptr)
	{
		return av_read_frame(this->m_FormatContext, outPacket);
	}
	return -1;
}

void FF::get_video_FrameData(AVCodecContext *InCodecContext, AVFrame *OutFrame, int *OutPos, AVPacket *InPacket)
{
	int ret = avcodec_decode_video2(
		InCodecContext,
		OutFrame,
		OutPos,
		InPacket);
	printf("getFrameData return code = %d\n", ret);
}

int FF::getAudioIndex(AVFormatContext *AVFC)
{
	int ret = -1;
	for (int i = 0; i < AVFC->nb_streams; i++)
	{
		if (AVFC->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			ret = i;
			return ret;
		}
	}
	return ret;
}

int FF::getVideoIndex(AVFormatContext *AVFC)
{
	int ret = -1;
	for (int i = 0; i < AVFC->nb_streams; i++)
	{
		if (AVFC->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			ret = i;
			return ret;
		}
	}
	return ret;
}

AVCodecContext *FF::getCodecContext(int index)
{
	return this->getCodecContext(this->m_FormatContext, index);
}

AVCodecContext *FF::getCodecContext(AVFormatContext *AVFC, int index)
{
	AVCodecContext *avcc = AVFC->streams[index]->codec;
	return avcc;
}