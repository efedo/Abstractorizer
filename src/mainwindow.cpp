#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qt3d/qt3d_imgui_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup widgets
    ImguiWindowQt3D * w = new ImguiWindowQt3D;
    w->resize(1280, 720);
    QWidget * widget = QWidget::createWindowContainer(w);
    QGridLayout *layout_qt3d = new QGridLayout;
    ui->tab_Qt3D->setLayout(layout_qt3d);
    layout_qt3d->addWidget(widget, 0, 0);
    widget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

