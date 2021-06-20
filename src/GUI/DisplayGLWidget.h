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

#ifndef DISPLAY_GLWIDGET_H_
#define DISPLAY_GLWIDGET_H_

#include <qtimer.h>

#include <QOpenGLWidget>
#include <opencv2/core.hpp>
#include <queue>

// A QtGLWidget that displays a texture on widget.
// Displays current frame that are sent/recived by stream controller.
class DisplayGLWidget : public QOpenGLWidget {
  Q_OBJECT

 public:
  // Constructor
  // @param QWidet* parent
  // Constructs a GLWidget under the parent qtwidget.
  DisplayGLWidget(QWidget *parent);

  // Destructor
  ~DisplayGLWidget() = default;

  // Sets current texture from given mat data.
  // @param cv::Mat mat
  void setTexture(const cv::Mat &mat);

 protected:
  // Callback function when GLWidget initialized.
  void initializeGL();

  // Callback function when GLWidget is updated.
  void paintGL();

  // Callback function when GLWidget is resized.
  // @param int width
  // New width.
  // @param int height
  // New height.
  void resizeGL(int width, int height);

 private:
  // Last displayed frame.
  cv::Mat last_frame;
  bool frame_updated;

  // Timer for automatically update window.
  QTimer timer;

  // OpenGL texture id for displaying frame data.
  GLuint texture_id;
};

#endif  // DISPLAY_GLWIDGET_H_
