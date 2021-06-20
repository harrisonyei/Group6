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

#ifndef STREAM_CONTROLLER_H_
#define STREAM_CONTROLLER_H_

#include <string>

class Render;
class Decoder;
class Router;
class Encoder;
class Capture;
class ScreenCapture;
class DisplayGLWidget;

template <typename T>
class Component;
typedef Component<int> ICapture;

// A class to control the streaming status
// Example:
// stream_controller = new Controller();
// // init
// stream_controller->init(ui.displayGLWidget);
// // start live
// stream_controller->live(port);
// // start watch
// stream_controller->watch(ip, listen_port, stream_port);
// // leave
// stream_controller->leave();

class Controller {
 public:
  enum class CaptureType { CAMERA = 0, SCREEN };

 public:
  // Default constructor
  Controller() = default;
  // Default destructor
  ~Controller() = default;
  // Set display widget pointer
  void init(DisplayGLWidget *);
  // Start streaming to target port
  void live(int);
  // Start receiving packet for source_ip:source_port and streaming to
  // output_port
  void watch(std::string source_ip, int source_port, int output_port);
  // Close streaming
  void leave();
  // Change current capture type.
  void changeType(CaptureType type);

 private:
  // Render component pointer
  Render *render;
  // Decoder component pointer
  Decoder *decoder;
  // Router component pointer
  Router *router;
  // Encoder component pointer
  Encoder *encoder;
  // Abstract Capture component pointer
  ICapture *capture;
  // Capture component pointer
  Capture *camera_capture;
  // ScreenCapture component pointer
  ScreenCapture *screen_capture;
  // Current capture type
  CaptureType capture_type;
};

#endif  // STREAM_CONTROLLER_H_
