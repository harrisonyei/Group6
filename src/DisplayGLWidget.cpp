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

#include "DisplayGLWidget.h"
#include <iostream>
#include <opencv2\imgproc.hpp>

DisplayGLWidget::DisplayGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	// add timer to automatically update window.
	// connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	// timer.start(10);
}

DisplayGLWidget::~DisplayGLWidget()
{
}

void DisplayGLWidget::setTexture(cv::Mat mat)
{
	// assign matrix data to last store frame.
	lastFrame = mat;
	// change data format from BGR to RGB.
	cv::cvtColor(lastFrame, lastFrame, cv::COLOR_RGB2BGR);
	// set QtWidget update flag.
	update();
}


void DisplayGLWidget::initializeGL()
{
	// generate texture for displaying sent/received data.
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set display-texture's configuration
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
}

void DisplayGLWidget::paintGL()
{
	float w = width();
	float h = height();
	// set viewport size.
	glViewport(0, 0, w, h);
	// clear buffers.
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set projection.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w, w, -h, h, -10, 10);

	// set modelview transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);

	// bind shader
	//this->commom_shader->Use();

	// bind texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// if frame data is not empty
	if (!lastFrame.empty()) {
		//cv::Mat frame(100, 100, CV_8UC3, cv::Scalar(0, 0, 255));
		int width = lastFrame.cols;
		int height = lastFrame.rows;

		unsigned char* image = lastFrame.data;
		// if data is not nullptr
		if (image)
		{
			// wrtie texture data from given image data.
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}

		// scale to fit screen
		float scale = std::min(w / (float)width, h / (float)height);
		glScalef(width * scale, height * scale, 1);
		// draw a quad with the given display-texture.
		glBegin(GL_QUADS); {
			glTexCoord2f(0, 1); glVertex2d(-1, -1);
			glTexCoord2f(0, 0); glVertex2d(-1, 1);
			glTexCoord2f(1, 0); glVertex2d(1, 1);
			glTexCoord2f(1, 1); glVertex2d(1, -1);

		} glEnd();
	}

	// bind vao
	
	// draw elements

	// done. unbind

	glDisable(GL_TEXTURE_2D);
}

void DisplayGLWidget::resizeGL(int width, int height)
{
	//[will handle resized event]
}
