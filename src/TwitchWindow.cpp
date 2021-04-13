#include "TwitchWindow.h"
#include "Components\Controller.h"

#include <sstream>
#include <iostream>
TwitchWindow::TwitchWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	streamController = new Controller();
	streamController->init();
}

TwitchWindow::~TwitchWindow()
{
	streamController->leave();
	delete streamController;
}

void TwitchWindow::onStartButtonClicked()
{
	std::cout << "start!\n";
	std::stringstream ss;
	ss << ui.stream_port_line->text().toStdString();

	std::cout << ui.stream_port_line->text().toStdString() << std::endl;
	int port;
	ss >> port;
	streamController->live(port);
}



void TwitchWindow::onLeaveButtonClicked()
{
	std::cout << "leave!\n";
	streamController->leave();
}

void TwitchWindow::onWatchButtonClicked()
{
	std::cout << "watch!\n";

	std::stringstream ss;
	std::string ip;
	ss << ui.ip_adress_line->text().toStdString();
	ss >> ip;
	ss.clear();

	int listen_port;
	ss << ui.listen_port_line->text().toStdString();
	ss >> listen_port;
	ss.clear();

	int stream_port;
	ss << ui.stream_port_line->text().toStdString();
	ss >> stream_port;
	ss.clear();

	streamController->watch(ip, listen_port, stream_port);
}
