#include <QApplication>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1280, 720);
    w.show();
    app.exec();
}
