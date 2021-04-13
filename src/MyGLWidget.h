#pragma once
#include <QOpenGLWidget>
#include <Qtimer>
#include <opencv2\core.hpp>
#include <queue>
class MyGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	MyGLWidget(QWidget *parent);
	~MyGLWidget();

	void setTexture(cv::Mat mat);
	 
protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	
private:
	cv::Mat lastFrame;
	QTimer timer;
	GLuint textureID;
};
