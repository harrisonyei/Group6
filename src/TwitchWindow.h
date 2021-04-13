#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TwitchWindow.h"

class Controller;
class TwitchWindow : public QMainWindow
{
    Q_OBJECT

public:
    TwitchWindow(QWidget *parent = Q_NULLPTR);
	~TwitchWindow();

public slots:
	void onStartButtonClicked();
	void onLeaveButtonClicked();
	void onWatchButtonClicked();

private:
    Ui::TwitchWindowClass ui;
	Controller* streamController;
};
