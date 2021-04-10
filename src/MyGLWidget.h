#pragma once
#include <QOpenGLWidget>
#include <opencv2\core.hpp>

class MyGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	MyGLWidget(QWidget *parent);
	~MyGLWidget();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

private:

	unsigned char* cvMat2glImg(cv::Mat& const mat);

	GLuint textureID;
};
