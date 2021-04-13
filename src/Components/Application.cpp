#include "Application.h"
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

using namespace std;
Controller* Application::controller = nullptr;

Application::Application()
{
	controller = new Controller();
	controller->init();
}
Application::~Application()
{
	controller->leave();
	delete controller;
}

void Application::start() {
	string cmd, ip;
	int port, outPort;
	cout << "command: ";
	while (cin >> cmd) {
		if (cmd == "live") {
			cout << "port: ";
			cin >> port;
			controller->live(port);
		}
		else if (cmd == "watch") {
			cout << "ip: ";
			cin >> ip;
			cout << "port: ";
			cin >> port;
			cout << "out port: ";
			cin >> outPort;
			controller->watch(ip, port, outPort);
		}
		else if (cmd == "leave") {
			controller->leave();
		}
		else {
			cout << "command not found.\n";
		}
		cout << "command: ";
	}
}