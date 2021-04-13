#pragma once
#include"Controller.h"
class Application {
public:
	Application();
	~Application();
	void start();
private:
	static Controller* controller;
};