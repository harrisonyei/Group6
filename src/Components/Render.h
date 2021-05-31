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

#ifndef STREAM_RENDER_H_
#define STREAM_RENDER_H_

#include "Component.h"
#include <mutex>
#include <queue>
#include <opencv2/opencv.hpp>

class DisplayGLWidget;

// A class show the cv::Mat to target window when living or watching.
// Example:
// Render* ren = new Capture(glWidget);
// // start living
// ren->run();
// // stop living
// ren->stop();
class Render: public Component<cv::Mat> {
public:
    // Constructors
    // Set target window pointer
    Render(DisplayGLWidget*);
protected:
    // Override component function
    // Get random window name
    void start();
    // Override component function
    // Return true if queue is enpty
    bool wait();
    // Override component function
    // Show cv::Mat on target window
    void process();
    // Override component function
    // Clear queue
    void end();
private:
    // OpenCV window name
    std::string name;
    // QTgui window pointer
    DisplayGLWidget* gl_widget;
};

#endif // STREAM_RENDER_H_
