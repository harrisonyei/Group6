#pragma once
#include "Component.h"
#include <mutex>
#include <queue>
#include <opencv2/opencv.hpp>

class MyGLWidget;

class Render: public stream::Component<cv::Mat> {
public:
	Render(MyGLWidget*);
protected:
	void start();
	bool wait();
	void process();
	void end();
private:
	std::string name;
	MyGLWidget* glWidget;
};
