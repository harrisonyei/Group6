#include "MyGLWidget.h"

#include <opencv2\imgproc.hpp>

MyGLWidget::MyGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
}

MyGLWidget::~MyGLWidget()
{
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

	glViewport(0, 0, width(), height());
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

	// transfer cvMat to glImg
	cv::Mat frame(100, 100, CV_8UC3, cv::Scalar(0, 0, 255));
	unsigned char* image = cvMat2glImg(frame);

	int width = frame.cols;
	int height = frame.rows;

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}

	glScalef(50, 50, 1);
	glBegin(GL_QUADS); {

	glTexCoord2f(0, 0); glVertex2d(-1, -1);
	glTexCoord2f(0, 1); glVertex2d(-1, 1);
	glTexCoord2f(1, 1); glVertex2d(1, 1);
	glTexCoord2f(1, 0); glVertex2d(1, -1);

	} glEnd();

	// bind vao
	
	// draw elements

	// done. unbind

	glDisable(GL_TEXTURE_2D);
}

void MyGLWidget::resizeGL(int width, int height)
{
}

unsigned char * MyGLWidget::cvMat2glImg(cv::Mat & const mat)
{
	cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
	return mat.data;
}
