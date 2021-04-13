#include "MyGLWidget.h"
#include <iostream>
#include <opencv2\imgproc.hpp>

MyGLWidget::MyGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.start(10);
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::setTexture(cv::Mat mat)
{
	lastFrame = mat;
	cv::cvtColor(lastFrame, lastFrame, cv::COLOR_RGB2BGR);
	update();
}


void MyGLWidget::initializeGL()
{
	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
}

void MyGLWidget::paintGL()
{
	// clear buffers
	float w = width();
	float h = height();

	glViewport(0, 0, w, h);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w, w, -h, h, -10, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);

	// bind shader
	//this->commom_shader->Use();

	// bind texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (!lastFrame.empty()) {
		//cv::Mat frame(100, 100, CV_8UC3, cv::Scalar(0, 0, 255));
		unsigned char* image = lastFrame.data;

		int width = lastFrame.cols;
		int height = lastFrame.rows;

		if (image)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}
		float scale = std::min(w / (float)width, h / (float)height);
		glScalef(width * scale, height * scale, 1);
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

void MyGLWidget::resizeGL(int width, int height)
{
}
