#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TwitchWindow.h"

class TwitchWindow : public QMainWindow
{
    Q_OBJECT

public:
    TwitchWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::TwitchWindowClass ui;
};
