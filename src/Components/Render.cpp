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

#include "Render.h"

#include "DisplayGLWidget.h"

Render::Render(DisplayGLWidget* _gl_widget) {
    gl_widget = _gl_widget;
}

void Render::start() {
    srand(time(NULL));
    name = std::to_string(rand());
}

bool Render::wait() {
    return getSize() == 0;
}

void Render::process() {
    std::shared_ptr<cv::Mat> frame;
    frame = getData();
    if (frame != nullptr) {
        if (glWidget != nullptr) {
            glWidget->setTexture(*frame);
        }
        // DEBUG
        cv::imshow(name, *frame);
        cv::waitKey(10);
    }
}

void Render::end() {
    while (getSize()) {
        getData();
    }
}
