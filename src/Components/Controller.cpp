#include "Controller.h"
#include "Capture.h"
#include "Render.h"
#include "Encoder.h"
#include "Decoder.h"
#include "Router.h"

void Controller::init(DisplayGLWidget* glWidget) {
	render = new Render(glWidget);
	decoder = new Decoder(render);
	router = new Router(decoder, this);
	encoder = new Encoder(router);
	capture = new Capture(render, encoder);

	// make sure initializing state
	router->stop();
	render->stop();
	decoder->stop();
	encoder->stop();
	capture->stop();
}
void Controller::live(int port) {
	std::cout << "try live" << ":" << port << std::endl;
	if (router->setPort(port)) {
		std::cout << "start live." << std::endl;
		decoder->stop();
		router->run();
		encoder->run();
		render->run();
		capture->run();
	}
}
void Controller::watch(std::string ip, int port, int outPort) {
	std::cout << "try watch " << ip << ":" << port << " , stream :" << outPort << std::endl;
	if (router->link(ip, port, outPort)) {
		std::cout << "start watch." << std::endl;
		capture->stop();
		encoder->stop();
		decoder->run();
		router->run();
		render->run();
	}
}
void Controller::leave() {
	std::cout << "leave.\n";
	router->stop();
	render->stop();
	decoder->stop();
	encoder->stop();
	capture->stop();
}
