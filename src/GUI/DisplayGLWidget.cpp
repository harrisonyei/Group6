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

#include "DisplayGLWidget.h"

#include <iostream>
#include <opencv2/imgproc.hpp>

DisplayGLWidget::DisplayGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
  // construct QtWidget
}

void DisplayGLWidget::setTexture(const cv::Mat& mat) {
  // check cv::mat format && last frame is drawn
  if (!frame_updated && mat.type() == CV_8UC3) {
    // assign matrix data to last store frame.
    last_frame = mat.clone();
    // set QtWidget update flag.
    update();
    // set frame updated flag.
    frame_updated = true;
  }
}

void DisplayGLWidget::initializeGL() {
  frame_updated = false;

  // generate texture for displaying sent/received data.
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  // set display-texture's configuration
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void DisplayGLWidget::paintGL() {
  if (!frame_updated) return;

  const float widget_width = width();
  const float widget_height = height();

  // set viewport size.
  glViewport(0, 0, widget_width, widget_height);

  // clear buffers.
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set projection.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-widget_width, widget_width, -widget_height, widget_height, -10, 10);

  // set modelview transformation
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);

  // [TODO] using shader to draw
  // bind shader
  // this->commom_shader->Use();

  // bind texture
  glBindTexture(GL_TEXTURE_2D, texture_id);

  // if frame data is not empty
  if (!last_frame.empty()) {
    const int& image_width = last_frame.cols;
    const int& image_height = last_frame.rows;

    const unsigned char* image = last_frame.data;
    // if data is not nullptr and frame is updated.
    if (image) {
      // wrtie texture data from given image data.
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0,
                   GL_BGR, GL_UNSIGNED_BYTE, image);
    }

    // scale to fit screen
    const float scale = std::min(widget_width / (float)image_width,
                                 widget_height / (float)image_height);
    glScalef(image_width * scale, image_height * scale, 1);
    // draw a quad with the given display-texture.
    glBegin(GL_QUADS);
    {
      glTexCoord2f(0, 1);
      glVertex2d(-1, -1);
      glTexCoord2f(0, 0);
      glVertex2d(-1, 1);
      glTexCoord2f(1, 0);
      glVertex2d(1, 1);
      glTexCoord2f(1, 1);
      glVertex2d(1, -1);
    }
    glEnd();
  }

  // [TODO] using shader to draw
  // bind vao
  // draw elements

  // done. unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);

  frame_updated = false;
}

void DisplayGLWidget::resizeGL(int width, int height) {}
