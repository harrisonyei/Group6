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

#include "Controller.h"

#include "Capture.h"
#include "Decoder.h"
#include "Encoder.h"
#include "Render.h"
#include "Router.h"
#include "ScreenCapture.h"

void Controller::init(DisplayGLWidget* glWidget) {
    // make a dataflow graph
    // capture -> encoder -> router -> decoder -> render
    // capture -> render
    render = new Render(glWidget);
    decoder = new Decoder(render);
    router = new Router(decoder, this);
    encoder = new Encoder(router);
    camera_capture = new Capture(render, encoder);
    screen_capture = new ScreenCapture(render, encoder);

    // make sure initializing state
    router->stop();
    render->stop();
    decoder->stop();
    encoder->stop();
    camera_capture->stop();
    screen_capture->stop();

    // default use camera capture
    capture_type = CaptureType::CAMERA;
    capture = camera_capture;
}

void Controller::live(int port) {
    std::cout << "try live" << ":" << port << std::endl;
    if (router->setPort(port)) {
        std::cout << "start live." << std::endl;
        decoder->stop();
        router->run();
        encoder->run();
        render->run();
        capture->run();
    }
}

void Controller::watch(std::string ip, int port, int outPort) {
    std::cout << "try watch " << ip << ":" << port << " , stream :" << outPort << std::endl;
    if (router->link(ip, port, outPort)) {
        std::cout << "start watch." << std::endl;
        capture->stop();
        encoder->stop();
        decoder->run();
        router->run();
        render->run();
    }
}

void Controller::leave() {
    std::cout << "leave.\n";
    router->stop();
    render->stop();
    decoder->stop();
    encoder->stop();
    capture->stop();
}

void Controller::changeType(CaptureType type)
{
    // Return if types are the same.
    if (capture_type == type)
        return;

    // cache for restoring running state after change capture type.
    bool running = capture->isActive();
    capture->stop();

    switch (type)
    {
    case Controller::CaptureType::CAMERA:
        capture = camera_capture;
        break;
    case Controller::CaptureType::SCREEN:
        capture = screen_capture;
        break;
    default:
        capture = camera_capture;
        break;
    }

    if (running)
    {
        capture->run();
    }
}
