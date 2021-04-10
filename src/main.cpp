#include "TwitchWindow.h"
#include <QtWidgets/QApplication>
#include "Components/Application.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TwitchWindow w;
    w.show();
    return a.exec();
}
