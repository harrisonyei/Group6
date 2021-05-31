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

#include "Decoder.h"

#include <assert.h>

Decoder::Decoder(Render* _render) {
    render = _render;
}

void Decoder::start() {
    if (init() < 0) {
        std::cerr << "decoder init failed.\n";
        stop();
    }
}

bool Decoder::wait() {
    return getSize() == 0;
}

void Decoder::process() {
    std::shared_ptr<cv::Mat> frame = std::make_shared<cv::Mat>();
    std::shared_ptr<H264> h264;
    h264 = getData();
    if (decode(h264->data, h264->size, *frame) == 0) {
        render->receive(frame);
    }
    else {
        std::cerr << "decode failed.\n";
    }
}

void Decoder::end() {
    avcodec_close(av_context);
    av_free(av_context);
    av_frame_free(&av_frame);
    clearData();
}

int Decoder::init()
{
    AVCodec* avCoder;
    avCoder = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!avCoder) {
        return -1;
    }
    av_context = avcodec_alloc_context3(avCoder);
    if (!av_context) {
        return -1;
    }
    av_context->pix_fmt = AV_PIX_FMT_YUV420P;
    av_context->codec_id = AV_CODEC_ID_H264;
    av_context->codec_type = AVMEDIA_TYPE_VIDEO;
    av_frame = av_frame_alloc();
    av_packet = av_packet_alloc();
    if (!av_frame || !av_packet) {
        return -1;
    }
    av_init_packet(av_packet);
    return avcodec_open2(av_context, avCoder, NULL);
}

int Decoder::decode(uint8_t* data, int size, cv::Mat& dist)
{
    if (size <= 0) {
        return -1;
    }
    av_packet->data = data;
    av_packet->size = size;
    if (avcodec_send_packet(av_context, av_packet) >= 0) {
        if (avcodec_receive_frame(av_context, av_frame) == 0) {
            dist.create(av_frame->height * 3 / 2, av_frame->width, CV_8UC1);
            assert(av_frame->width * av_frame->height * 5 / 4 + av_frame->width * av_frame->height / 4 <= dist.total() * dist.elemSize());
            memcpy(dist.data, av_frame->data[0], av_frame->width * av_frame->height);
            memcpy(dist.data + av_frame->width * av_frame->height, av_frame->data[1], av_frame->width * av_frame->height / 4);
            memcpy(dist.data + av_frame->width * av_frame->height * 5 / 4, av_frame->data[2], av_frame->width * av_frame->height / 4);
            cv::cvtColor(dist, dist, cv::COLOR_YUV2BGR_I420);
            return 0;
        }
    }
    return -1;
}
