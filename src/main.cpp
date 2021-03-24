// (c) 2021 Eric Fedosejevs and contributors. Licensed under MIT license (see LICENSE for details)

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
