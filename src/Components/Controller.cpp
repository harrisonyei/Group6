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

void Controller::init(DisplayGLWidget* glWidget) {
    // make a dataflow graph
    // capture -> encoder -> router -> decoder -> render
    // capture -> render
    render = new Render(glWidget);
    decoder = new Decoder(render);
    router = new Router(decoder, this);
    encoder = new Encoder(router);
    capture = new Capture(render, encoder);

    // make sure initializing state
    router->stop();
    render->stop();
    decoder->stop();
    encoder->stop();
    capture->stop();
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
