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

#include "ScreenCapture.h"

#include "Decoder.h"
#include "Encoder.h"
#include "Render.h"

ScreenCapture::ScreenCapture(Render* render_, Encoder* encoder_) : Component() {
    render = render_;
    encoder = encoder_;
    // initialize
    screen = nullptr;
    img_width = 800;
}

void ScreenCapture::start() {
    setupWindow();
}

bool ScreenCapture::wait() {
    return false;
}

void ScreenCapture::process() {
    std::shared_ptr<cv::Mat> framePtr(new cv::Mat());
    // capture image
    (*framePtr) = captureScreenMat();
    // send to renderer and encoder
    if (!framePtr->empty()) {
        render->receive(framePtr);
        encoder->receive(framePtr);
    }
}

void ScreenCapture::end() {
    releaseWindow();
}

void ScreenCapture::setupWindow(){
    screen = QGuiApplication::primaryScreen();
}

void ScreenCapture::releaseWindow(){
    screen = nullptr;
}

cv::Mat ScreenCapture::captureScreenMat()
{
    if (!screen)
		return cv::Mat();

	screen_pixmap = screen->grabWindow(0).scaledToWidth(img_width);
    
	screen_image = screen_pixmap.toImage().convertToFormat(QImage::Format::Format_BGR888);

	cv::Mat mat(screen_image.height(), screen_image.width(), CV_8UC3, (cv::Scalar*)screen_image.scanLine(0));

    return mat;
}
