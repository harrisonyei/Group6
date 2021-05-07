#pragma once
#include <string>
class Render;
class Decoder;
class Router;
class Encoder;
class Capture;
class DisplayGLWidget;

class Controller {
public:
	void init(DisplayGLWidget*);
	void live(int);
	void watch(std::string, int, int);
	void leave();
private:
	Render* render;
	Decoder* decoder;
	Router* router;
	Encoder* encoder;
	Capture* capture;
};
