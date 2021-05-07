#pragma once
#include "Component.h"
#include <mutex>
#include <queue>
#include <opencv2/opencv.hpp>

class DisplayGLWidget;

class Render: public Component<cv::Mat> {
public:
	Render(DisplayGLWidget*);
protected:
	void start();
	bool wait();
	void process();
	void end();
private:
	std::string name;
	DisplayGLWidget* glWidget;
};
