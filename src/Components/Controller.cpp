#include "Controller.h"
#include "Capture.h"
#include "Render.h"
#include "Encoder.h"
#include "Decoder.h"
#include "Router.h"

void Controller::init() {
	render = new Render();
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
	cout << "try live" << ":" << port << std::endl;
	if (router->setPort(port)) {
		cout << "start live." << std::endl;
		decoder->stop();
		router->run();
		encoder->run();
		render->run();
		capture->run();
	}
}
void Controller::watch(string ip, int port, int outPort) {
	cout << "try watch " << ip << ":" << port << " , stream :" << outPort << std::endl;
	if (router->link(ip, port, outPort)) {
		cout << "start watch." << std::endl;
		capture->stop();
		encoder->stop();
		decoder->run();
		router->run();
		render->run();
	}
}
void Controller::leave() {
	cout << "leave.\n";
	router->stop();
	render->stop();
	decoder->stop();
	encoder->stop();
	capture->stop();
}