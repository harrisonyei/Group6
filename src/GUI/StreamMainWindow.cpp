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

#include "StreamMainWindow.h"

#include <iostream>
#include <regex>
#include <sstream>

#include "Components/Controller.h"

StreamMainWindow::StreamMainWindow(QWidget *parent) : QMainWindow(parent) {
  // setup UIs of the widget.
  ui.setupUI(this);

  // Allocate a stream controller, then initialize it with the widget to be
  // updated.
  stream_controller = new Controller();
  stream_controller->init(ui.display_glwidget);
}

StreamMainWindow::~StreamMainWindow() {
  // stops stream
  stream_controller->leave();
  delete stream_controller;
  stream_controller = nullptr;
}

void StreamMainWindow::onStartButtonClicked() {
  // handle start-button clicked event.
  std::cout << "start!\n";

  // read input from GUI.
  // port to stream
  std::stringstream ss;
  ss << ui.stream_port_line->text().toStdString();
  uint16_t port;
  ss >> port;

  // set controller to go live at the given port.
  stream_controller->live(port);
}

void StreamMainWindow::onLeaveButtonClicked() {
  // handle leave-button clicked event.
  std::cout << "leave!\n";
  // leave / stop current stream
  stream_controller->leave();
}

// Regex expression for validating IPv4
const std::regex IPV4_REGEX(
    "(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-"
    "9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
// Regex expression for validating integer number.
const std::regex NUM_REGEX("[0-9]{1,6}");

void StreamMainWindow::onWatchButtonClicked() {
  // handle watch-button clicked event.
  std::cout << "watch!\n";
  // read input from GUI
  // remote ip address to watch stream
  std::string ip = ui.ip_adress_line->text().toStdString();

  // Checking if it is a valid IPv4 addresses
  if (!regex_match(ip, IPV4_REGEX)) {
    // invalid ip address
    std::cout << "Invalid ip address." << std::endl;
    return;
  }

  // remote port to listen
  std::string listen_port_str = ui.listen_port_line->text().toStdString();
  if (!regex_match(listen_port_str, NUM_REGEX)) {
    // invalid ip address
    std::cout << "Invalid listen port." << std::endl;
    return;
  }
  uint16_t listen_port = std::stoi(listen_port_str);

  // port to stream
  std::string stream_port_str = ui.stream_port_line->text().toStdString();
  if (!regex_match(listen_port_str, NUM_REGEX)) {
    // invalid ip address
    std::cout << "Invalid stream port." << std::endl;
    return;
  }
  uint16_t stream_port = std::stoi(stream_port_str);

  // set controller to start watching a stream and sending received frame data
  // to other downstream users.
  stream_controller->watch(ip, listen_port, stream_port);
}

void StreamMainWindow::onSelectCaptureType(int index) {}
