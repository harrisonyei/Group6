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

#ifndef STREAM_SCREEN_CAPTURE_H_
#define STREAM_SCREEN_CAPTURE_H_

#include "Component.h"

#include <windows.h>

#include <opencv2/opencv.hpp>

class Encoder;
class Render;

// A base class to get cv::Mat.
// The default is to read from the windows handler.
// Example:
// ScreenCapture* scr_cap = new ScreenCapture(render, encoder);
// // start living
// scr_cap->run();
// // stop living
// scr_cap->stop();
class ScreenCapture: public Component<int> {
public:
    // Constructors
    // Set render pointer and encoder pointer
    ScreenCapture(Render*, Encoder*);
protected:
    // Override component function
    // Setup windows screen capture variables
    void start();
    // Override component function
    // Return false
    bool wait();
    // Override component function
    // Get cv::Mat to shared_ptr and send to render and encoder if it is not enpty
    void process();
    // Override component function
    // Release cached windows variables
    void end();
private:
    // Setup window configurations and context, and create bitmap for copying screen content.
    void setupWindow();
    // Release stored window context and bitmap.
    void releaseWindow();
    // Capture screen content and return as cv::Mat
    cv::Mat captureScreenMat();

    // Render component pointer
    Render* render;
    // Encoder component pointer
    Encoder* encoder;
private:
    struct WNDConfig{
        int x; // screen x position
        int y; // screen y position
        int w; // screen width
        int h; // screen height
    };
    // Window configuration
    WNDConfig window_config;
    // Monitor configuration
    WNDConfig monitor_config;
    // Window handle
    HWND hwnd;
    // Device context
    HDC hwindowDC;
    // Compatible device context for bitmap
    HDC hwindowCompatibleDC;
    // Bitmap for storing screen content 
    HBITMAP bitmap;
    // Bitmap header which defines configurations of bitmap
    BITMAPINFOHEADER bitmap_info;

    // output image width
    int img_width;
    // output image height
    int img_height;
};

#endif // STREAM_SCREEN_CAPTURE_H_
