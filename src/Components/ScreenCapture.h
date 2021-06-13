// MIT License

// Copyright (c) 2021 YeiTingHao ShiChengFeng

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef STREAM_SCREEN_CAPTURE_H_
#define STREAM_SCREEN_CAPTURE_H_

#include <QtWidgets>
#include <opencv2/opencv.hpp>

#include "Component.h"

class Encoder;
class Render;

// A base class to get cv::Mat.
// The default is to read from the screen handler.
// Example:
// ScreenCapture* scr_cap = new ScreenCapture(render, encoder);
// // start living
// scr_cap->run();
// // stop living
// scr_cap->stop();
class ScreenCapture : public Component<int> {
 public:
  // Constructors
  // Set render pointer and encoder pointer
  ScreenCapture(Render*, Encoder*);

 protected:
  // Override component function
  // Setup window screen capture variables
  void start();
  // Override component function
  // Return false
  bool wait();
  // Override component function
  // Get cv::Mat to shared_ptr and send to render and encoder if it is not enpty
  void process();
  // Override component function
  void end();

 private:
  // Setup window configurations and context, and create bitmap for copying
  // screen content.
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
  // screen handler
  QScreen* screen;
  // store scaled pixelmap of the screen
  QPixmap screen_pixmap;
  // store captured image
  QImage screen_image;
  // output image width
  int img_width;
};

#endif  // STREAM_SCREEN_CAPTURE_H_
