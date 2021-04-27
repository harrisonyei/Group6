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

#include "StreamMainWindow.h"
#include "Components\Controller.h"

#include <sstream>
#include <iostream>
StreamMainWindow::StreamMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	// setup UIs of the widget.
    ui.setupUi(this);

	// Allocate a stream controller, then initialize it with the widget to be updated.
	streamController = new Controller();
	streamController->init(ui.DisplayGLWidget);
}

StreamMainWindow::~StreamMainWindow()
{
	// stops stream
	streamController->leave();
	delete streamController;
}

void StreamMainWindow::onStartButtonClicked()
{
	// handle start-button clicked event.
	std::cout << "start!\n";

	// read input from GUI.
	// port to stream
	std::stringstream ss;
	ss << ui.stream_port_line->text().toStdString();
	int port;
	ss >> port;

	// set controller to go live at the given port.
	streamController->live(port);
}

void StreamMainWindow::onLeaveButtonClicked()
{
	// handle leave-button clicked event.
	std::cout << "leave!\n";
	// leave / stop current stream
	streamController->leave();
}

void StreamMainWindow::onWatchButtonClicked()
{
	// handle watch-button clicked event.
	std::cout << "watch!\n";

	// read input from GUI
	std::stringstream ss;
	// remote ip address to watch stream
	std::string ip;
	ss << ui.ip_adress_line->text().toStdString();
	ss >> ip;
	ss.clear();
	// remote port to listen
	int listen_port;
	ss << ui.listen_port_line->text().toStdString();
	ss >> listen_port;
	ss.clear();
	// port to stream
	int stream_port;
	ss << ui.stream_port_line->text().toStdString();
	ss >> stream_port;
	ss.clear();

	// set controller to start watching a stream and sending received frame data to other downstream users.
	streamController->watch(ip, listen_port, stream_port);
}
