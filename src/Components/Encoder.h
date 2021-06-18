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

#ifndef STREAM_ENCODER_H_
#define STREAM_ENCODER_H_

extern "C" {
#include <libavformat/avformat.h>
};
#include <opencv2/opencv.hpp>

#include "Component.h"

class Router;

// A class encode cv::Mat to H264.
// Example:
// Encoder* enc = new Encoder(router);
// // start encoding
// enc->run();
// // stop encoding
// enc->stop();
class Encoder: public Component<cv::Mat> {
public:
    // Constructors
    // Set router pointer
    Encoder(Router*);
protected:
    // Override component function
    // Alloc avContext, avFrame and avPacket, set libav encoding config
    void start();
    // Override component function
    // Return true if queue is enpty
    bool wait();
    // Override component function
    // Encode cv::Mat to H264 packet and send to router
    void process();
    // Override component function
    // Free avContext, avFrame, avPacket and clear queue
    void end();
private:
    // Router component pointer
    Router* router;
    // Libav context pointer
    AVCodecContext* av_context;
    // Libav frame pointer
    AVFrame* av_frame;
    // Libav packet pointer
    AVPacket* av_packet;
    // Encode frame index counter
    int frameIndex;
    // Set libav encoding config and return 0 if succeeded
    int init(struct EncodeConfig);
    // Using libav to encode cv::Mat and return H264 packet pointer
    AVPacket* encode(const cv::Mat&);
};

#endif // STREAM_ENCODER_H_
