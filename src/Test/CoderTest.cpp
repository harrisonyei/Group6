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

#include <opencv2/opencv.hpp>

#include "Components/Decoder.h"
#include "Components/Encoder.h"
#include "Components/Render.h"
#include "Components/Router.h"
#include "gtest/gtest.h"

// A class that forwards data from the encoder to the decoder
class MockRouter : public Router {
 public:
  // Constructors
  // Set decoder pointer
  MockRouter(Decoder*);

 protected:
  // Override component function
  // Clear queue
  void start();
  // Override component function
  // Return true if queue is enpty
  bool wait();
  // Override component function
  // Send data to decoder
  void process();
  // Override component function
  // Clear queue
  void end();

 private:
  Decoder* decoder;
};

MockRouter::MockRouter(Decoder* _decoder) : Router(nullptr, nullptr) {
  decoder = _decoder;
}

void MockRouter::start() { clearData(); }

bool MockRouter::wait() { return getSize() == 0; }

void MockRouter::process() { decoder->receive(getData()); }

void MockRouter::end() { clearData(); }

// A test class that send the frame to the encoder and compare it after
// receiving the frame from the decoder
class MockRender : public Render {
 public:
  // Create encode, router, decoder and initialization variable.
  MockRender();

 protected:
  // Override component function
  // Read frame data from "test.gif", and send to encoder after activate
  // encoder, router and decoder.
  void start();
  // Override component function
  // Return true if queue is enpty
  bool wait();
  // Override component function
  // Compare original frame and compressed frame
  void process();
  // Override component function
  // Clear data and deactivate encoder, router and decoder.
  void end();

 private:
  Encoder* encoder;
  MockRouter* router;
  Decoder* decoder;
  std::vector<cv::Mat> all_frames;
  int index;
};

MockRender::MockRender() : Render(nullptr) {
  decoder = new Decoder(this);
  router = new MockRouter(decoder);
  encoder = new Encoder(router);
  all_frames.clear();
  index = 0;
}

void MockRender::start() {
  // Open test gif file
  cv::VideoCapture video_capture("test.gif");
  if (!video_capture.isOpened()) {
    stop();
  }
  ASSERT_TRUE(video_capture.isOpened());
  // Activate components
  encoder->run();
  router->run();
  decoder->run();
  // Get all frame in gif
  cv::Mat frame;
  int index = 0;
  std::shared_ptr<cv::Mat> framePtr = std::make_shared<cv::Mat>();
  while (video_capture.read(*framePtr)) {
    cv::Mat frame;
    framePtr->copyTo(frame);
    // Save original frame
    all_frames.push_back(frame);
    // Send to encoder -> mockrouter -> decoder
    encoder->receive(framePtr);
    framePtr = std::make_shared<cv::Mat>();
  }
  video_capture.release();
}

bool MockRender::wait() { return getSize() == 0; }

void MockRender::process() {
  std::shared_ptr<cv::Mat> frame;
  frame = getData();
  if (frame != nullptr && index < all_frames.size()) {
    cv::Mat resize_frame;
    cv::resize(all_frames[index], resize_frame,
               cv::Size(frame->cols, frame->rows));
    // calu PSNR
    cv::Mat diff;
    absdiff(resize_frame, *frame, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);
    cv::Scalar s = sum(diff);
    double sse = s.val[0] + s.val[1] + s.val[2];
    if (sse < 1e-10) {
      sse = 1e-10;
    }
    double mse = sse / (double)(diff.channels() * diff.total());
    double psnr = 10.0 * log10((255.0 * 255.0) / mse);
    EXPECT_GE(psnr, 20);
    index++;
    if (index == all_frames.size() - 1) {
      stop();
    }
  }
}

void MockRender::end() {
  all_frames.clear();
  clearData();
  encoder->stop();
  router->stop();
  decoder->stop();
}

// Calculate PSNR value after encoding and decoding a GIF file, If it exceeds
// 20, it will pass the test.
TEST(Encoder_Decoder, EncodeAndDecodeGIF) {
  MockRender* render;
  render = new MockRender();
  render->run();
  while (render->isActive())
    ;
}
