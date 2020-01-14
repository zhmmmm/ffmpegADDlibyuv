#ifndef __FF_H__
#define __FF_H__

extern "C"
{}


#ifdef FFMPEGFORWINDOWS
	#ifdef _WIN64
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
