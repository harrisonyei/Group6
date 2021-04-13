#pragma once
#include "Component.h"
#include "MyGLWidget.h"
#include <mutex>
#include <queue>
#include <opencv2/opencv.hpp>
class Render: public Component<cv::Mat> {
public:
	Render(MyGLWidget*);
protected:
	void start();
	bool wait();
	void process();
	void end();
private:
	string name;
	MyGLWidget* glWidget;
};
