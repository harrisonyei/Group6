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
    ui.setupUi(this);
	streamController = new Controller();
	streamController->init(ui.DisplayGLWidget);
}

StreamMainWindow::~StreamMainWindow()
{
	streamController->leave();
	delete streamController;
}

void StreamMainWindow::onStartButtonClicked()
{
	std::cout << "start!\n";
	std::stringstream ss;
	ss << ui.stream_port_line->text().toStdString();

	std::cout << ui.stream_port_line->text().toStdString() << std::endl;
	int port;
	ss >> port;
	streamController->live(port);
}

void StreamMainWindow::onLeaveButtonClicked()
{
	std::cout << "leave!\n";
	streamController->leave();
}

void StreamMainWindow::onWatchButtonClicked()
{
	std::cout << "watch!\n";

	std::stringstream ss;
	std::string ip;
	ss << ui.ip_adress_line->text().toStdString();
	ss >> ip;
	ss.clear();

	int listen_port;
	ss << ui.listen_port_line->text().toStdString();
	ss >> listen_port;
	ss.clear();

	int stream_port;
	ss << ui.stream_port_line->text().toStdString();
	ss >> stream_port;
	ss.clear();

	streamController->watch(ip, listen_port, stream_port);
}
