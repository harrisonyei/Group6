#include "StreamMainWindow.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StreamMainWindow w;
    w.show();
    return a.exec();
}
