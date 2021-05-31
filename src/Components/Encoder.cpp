// MIT License

// Copyright (c) 2021 YeiTingHao ShiChengFeng

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Encoder.h"

#include <iostream>
#include <memory>

#include "DataType.h"

Encoder::Encoder(Router* _router) {
    router = _router;
}

void Encoder::start() {
    EncodeConfig config;
    if (init(config) < 0) {
        std::cerr << "encoder init failed.\n";
        stop();
    }
}

bool Encoder::wait() {
    return getSize() == 0;
}

void Encoder::process() {
    std::shared_ptr<cv::Mat> frame;
    frame = getData();
    if (encode(*frame)) {
        std::shared_ptr<H264> h264 = std::make_shared<H264>();
        h264->size = av_packet->size;
        h264->data = new uint8_t[h264->size];
        if (h264->data) {
            memcpy(h264->data, av_packet->data, h264->size);
            router->receive(h264);
        }
        else {
            std::cerr << "allocate packet failed.\n";
        }
    }
    else {
        std::cerr << "encode failed.\n";
    }
}

void Encoder::end() {
    if (av_context) {
        avcodec_free_context(&av_context);
    }
    if (av_frame) {
        av_frame_free(&av_frame);
    }
    if (av_packet) {
        av_packet_free(&av_packet);
    }
    clearData();
}

int Encoder::init(EncodeConfig config) {
    AVCodec* avCoder;
    avCoder = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!avCoder) {
        return -1;
    }
    av_context = avcodec_alloc_context3(avCoder);
    if (!av_context) {
        return -1;
    }
    av_context->width = config.width;
    av_context->height = config.height;
    av_context->time_base = { 1, config.frame_rate };
    av_context->framerate = { config.frame_rate, 1 };
    av_context->gop_size = config.gop_size;
    av_context->max_b_frames = config.max_b_frames;
    av_context->pix_fmt = AV_PIX_FMT_YUV420P;
    av_context->codec_id = AV_CODEC_ID_H264;
    av_context->codec_type = AVMEDIA_TYPE_VIDEO;
    AVDictionary* dict = nullptr;
    av_dict_set(&dict, "preset", "veryfast", 0);
    av_dict_set(&dict, "tune", "zerolatency", 0);
    av_dict_set(&dict, "profile", "main", 0);
    av_frame = av_frame_alloc();
    av_packet = av_packet_alloc();
    if (!av_frame || !av_packet) {
        return -1;
    }
    av_init_packet(av_packet);
    av_frame->format = av_context->pix_fmt;
    av_frame->width = av_context->width;
    av_frame->height = av_context->height;
    if (av_frame_get_buffer(av_frame, 0) < 0) {
        return -1;
    }
    if (av_frame_make_writable(av_frame) < 0) {
        return -1;
    }
    frameIndex = 0;
    return avcodec_open2(av_context, avCoder, &dict);
}

AVPacket* Encoder::encode(const cv::Mat& src) {
    if (src.empty()) {
        return NULL;
    }
    cv::Mat temp, dist;
    cv::resize(src, temp, cv::Size(av_context->width, av_context->height));
    cv::cvtColor(temp, dist, cv::COLOR_BGR2YUV_I420);
    av_frame->data[0] = dist.data;
    av_frame->data[1] = dist.data + av_context->width * av_context->height;
    av_frame->data[2] = dist.data + av_context->width * av_context->height * 5 / 4;
    av_frame->pts = frameIndex++;
    if (avcodec_send_frame(av_context, av_frame) >= 0) {
        if (avcodec_receive_packet(av_context, av_packet) == 0) {
            av_packet->stream_index = av_frame->pts;
            return av_packet;
        }
    }
    return NULL;
}
