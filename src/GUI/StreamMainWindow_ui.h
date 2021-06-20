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

#ifndef STREAM_MAIN_WINDOW_UI_H_
#define STREAM_MAIN_WINDOW_UI_H_

#include <QtWidgets>

class StreamMainWindow;
class DisplayGLWidget;

// StreamMainWindowUI is for creating qt gui for StreamMainWindow class.
class StreamMainWindowUI {
 public:
  // setup all UIs.
  void setupUI(StreamMainWindow* widget);
  // setup menu items.
  void setupMenus();
  // setup widgets.
  void setupWidgets();
  // setup displayGLWidget.
  QWidget* setupDisplayWidget(QWidget*);
  // setup input text fields.
  QWidget* setupInputWidget(QWidget*);
  // setup buttons.
  QWidget* setupButtonWidget(QWidget*);

  // store the StreamMainWindow it handles.
  StreamMainWindow* main_widget;

  // store displayGLWidget .
  DisplayGLWidget* display_glwidget;

  // store input fields.
  QLineEdit* stream_port_line;
  QLineEdit* listen_port_line;
  QLineEdit* ip_adress_line;
  QComboBox* capture_type_combo;

};

#endif  // STREAM_MAIN_WINDOW_UI_H_
