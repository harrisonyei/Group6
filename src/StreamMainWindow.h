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

#ifndef STREAM_MAIN_WINDOW_H_
#define STREAM_MAIN_WINDOW_H_

#include "ui_StreamMainWindow.h"

#include <QtWidgets/QMainWindow>

// Controller class declaration, avoiding include loop.
class Controller;

// A QtMainWindow that contains a stream controller and a GUI widget.
// Handles the interactions between user gui events and controller.
class StreamMainWindow : public QMainWindow
{
    Q_OBJECT
public:
	// Constructor
	// @param QWidet* parent
	// Constructs a qtwidget under the parent qtwidget.
	// Creates a stream contoller, then initializes it.
	StreamMainWindow(QWidget *parent = Q_NULLPTR);

	// Descturctor
	// Sets stream controller to stop the stream, then delete the controller.
	~StreamMainWindow();

// Qt callbacks
public slots:
	// Handles Start-button clicked event.
	// Starts streaming from a user given port.
	void onStartButtonClicked();

	// Handles Leave-button clicked event.
	// Leaves / Stops a live stream.
	void onLeaveButtonClicked();

	// Handles Watch-button clicked event.
	// Starts watching a live stream that is from the given ip and port.
	void onWatchButtonClicked();

private:
	// UI configurations of this window.
	Ui::StreamMainWindowClass ui;

	// Controller that controls states of streaming components.
	Controller* stream_controller;

};

#endif // STREAM_MAINWINDOW_WINDOW_H_
