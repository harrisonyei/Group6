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

#include "Capture.h"

#include "Decoder.h"
#include "Encoder.h"
#include "Render.h"

Capture::Capture(Render* render_, Encoder* encoder_) : Component() {
    render = render_;
    encoder = encoder_;
}

void Capture::start() {
    video_capture.open(0);
}

bool Capture::wait() {
    return false;
}

void Capture::process() {
    std::shared_ptr<cv::Mat> framePtr(new cv::Mat());
    video_capture >> (*framePtr);

    // images from cv::Capture are in BGR color space. Convert them to RGB instead.
    cv::cvtColor((*framePtr), (*framePtr), cv::COLOR_BGR2RGB);

    if (!framePtr->empty()) {
        render->receive(framePtr);
        encoder->receive(framePtr);
    }
}

void Capture::end() {
    video_capture.release();
}
