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

#ifndef STREAM_DECODER_H_
#define STREAM_DECODER_H_

extern "C" {
#include "libavformat/avformat.h"
}
#include <opencv2/opencv.hpp>

#include "Component.h"
#include "DataType.h"
#include "Render.h"

// A class decode H264 to cv::Mat.
// Example:
// Decoder* dec = new Decoder(render, encoder);
// // start decoding
// dec->run();
// // stop decoding
// dec->stop();
class Decoder: public Component<H264> {
public:
    // Constructors
    // Set render pointer
    Decoder(Render*);
public:
    // Override component function
    // Alloc avContext, avFrame and avPacket
    void start();
    // Override component function
    // Return true if queue is enpty
    bool wait();
    // Override component function
    // Decode H264 packet to cv::Mat and send to render
    void process();
    // Override component function
    // Free avContext, avFrame, avPacket and clear queue
    void end();
private:
    // Render component pointer
    Render* render;
    // Libav context pointer
    AVCodecContext* av_context;
    // Libav frame pointer
    AVFrame* av_frame;
    // Libav packet pointer
    AVPacket* av_packet;
    // Init libav pointers, called in start
    int init();
    // Using libav to decode H264 packet and return cv::Mat
    int decode(uint8_t*, int, cv::Mat&);
};

#endif // STREAM_DECODER_H_
