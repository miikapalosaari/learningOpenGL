extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}
#include <iostream>
#include <vector>
#include <common/application.h>

class FFMPEG_TEST_APP : public Application 
{
public:
	FFMPEG_TEST_APP() : Application(640, 480, "FFMPEG Test Application") 
	{
		filename = "../assets/pacman_demo.mp4";
		formatContext = avformat_alloc_context();

		loadVideo(filename);
		retrieveStreamInfo(formatContext);
		//printStreamInfo();
		initDecoder();

		frame = av_frame_alloc();
		packet = av_packet_alloc();
		swsContext = sws_getContext
		(
			codecContext->width, 
			codecContext->height, 
			codecContext->pix_fmt,
			codecContext->width,
			codecContext->height,
			AV_PIX_FMT_RGB24,
			SWS_BILINEAR,
			nullptr,
			nullptr,
			nullptr
		);
	}

	~FFMPEG_TEST_APP()
	{
		if (codecContext) avcodec_free_context(&codecContext);
		if (formatContext) avformat_close_input(&formatContext);
		if (frame) av_frame_free(&frame);
		if (packet) av_packet_free(&packet);
		if (swsContext) sws_free_context(&swsContext);
	}

	bool loadVideo(const char* filename)
	{
		if (avformat_open_input(&formatContext, filename, nullptr, nullptr) != 0)
		{
			std::cerr << "Error: Could not open file " << filename << std::endl;
			return false;
		}
		return true;
	}

	bool retrieveStreamInfo(AVFormatContext* formatContext)
	{
		if (avformat_find_stream_info(formatContext, nullptr) < 0) 
		{
			std::cout << "Could not find stream information" << std::endl;
			avformat_close_input(&formatContext);
			return false;
		}
		return true;
	}

	void printStreamInfo()
	{
		int64_t duration_sec = formatContext->duration / AV_TIME_BASE;
		int hours = duration_sec / 3600;
		int minutes = (duration_sec % 3600) / 60;
		int seconds = duration_sec % 60;

		std::cout << "Duration: " << "HH:" << hours << " MM:" << minutes << " SS:" << seconds << std::endl;
		av_dump_format(formatContext, 0, filename, 0); // Printing info
	}

	bool initDecoder()
	{
		for (unsigned int i = 0; i < formatContext->nb_streams; i++)
		{
			if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				videoStreamIndex = i;
				break;
			}
		}

		if (videoStreamIndex == -1)
		{
			std::cout << "Could not find a video stream" << std::endl;
			return false;
		}

		AVCodecParameters* codecParams = formatContext->streams[videoStreamIndex]->codecpar;
		const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
		if (!codec)
		{
			std::cout << "Unsupported codec" << std::endl;
			return false;
		}

		codecContext = avcodec_alloc_context3(codec);
		avcodec_parameters_to_context(codecContext, codecParams);
		if(avcodec_open2(codecContext, codec, nullptr) < 0)
		{
			std::cout << "Could not open codec" << std::endl;
			return false;
		}
		return true;
	}

	bool decodeNextFrame(std::vector<uint8_t>& rgbBuffer)
	{
		while (av_read_frame(formatContext, packet) >= 0)
		{
			if (packet->stream_index == videoStreamIndex)
			{
				if (avcodec_send_packet(codecContext, packet) == 0)
				{
					while (avcodec_receive_frame(codecContext, frame) == 0)
					{
						int width = codecContext->width;
						int height = codecContext->height;
						rgbBuffer.resize(3 * width * height);

						uint8_t* rgbData[1] = {rgbBuffer.data()};
						int rgbLineSize[1] = {3 * width};
						
						sws_scale(swsContext, frame->data, frame->linesize, 0, height, rgbData, rgbLineSize);
						av_packet_unref(packet);
						return true;
					}
				}
			}
			av_packet_unref(packet);
		}
		return false;
	}

	void handleInput(GLFWwindow* window) override
	{

	}

	void update(float deltaTime) override
	{
		if (decodeNextFrame(rgbBuffer))
		{
			std::cout << "Decoded frame of size: " << rgbBuffer.size() << std::endl;
		}
	}

	void render(Renderer& renderer) override
	{

	}

private:
	const char* filename;
	AVFormatContext* formatContext;
	int videoStreamIndex = -1;
	AVCodecContext* codecContext = nullptr;

	AVFrame* frame = nullptr;
	AVPacket* packet = nullptr;
	struct SwsContext* swsContext = nullptr;

	std::vector<uint8_t> rgbBuffer;
};

int main()
{
	FFMPEG_TEST_APP app;
	return app.run();
}
