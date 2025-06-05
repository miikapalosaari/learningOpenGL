extern "C" {
#include <libavformat/avformat.h>
}

#include <iostream>
#include <filesystem>

int main() {
    const char* filename;
    filename = "../assets/pacman_demo.mp4";
    std::cout << "No arguments provided. Using default: " << filename << std::endl;

    AVFormatContext* fmt_ctx = nullptr;

    // Open input file and allocate format context
    if (avformat_open_input(&fmt_ctx, filename, nullptr, nullptr) != 0) {
        printf("Could not open file: %s\n", filename);
        return -1;
    }

    // Retrieve stream information
    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        printf("Could not find stream information\n");
        avformat_close_input(&fmt_ctx);
        return -1;
    }

		int64_t duration_sec = fmt_ctx->duration / AV_TIME_BASE;
		int hours = duration_sec / 3600;
		int minutes = (duration_sec % 3600) / 60;
		int seconds = duration_sec % 60;

		printf("Duration: %02d:%02d:%02d\n", hours, minutes, seconds);

    // Print summary
    av_dump_format(fmt_ctx, 0, filename, 0);

    avformat_close_input(&fmt_ctx);
    return 0;
}
