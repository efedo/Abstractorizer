#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "generic/engine_window.h"
#include "qt3d/qt3d_window.h"
#include "urho3d/urho3d_window.h"

#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _addWindowTab(QWidget::createWindowContainer(new Qt3DWindow), "Qt3D");
    _addWindowTab(new Urho3DWindow, "Urho3D");

    Urho3D::SharedPtr<Urho3D::Context> context = new Urho3D::Context();
    Urho3dWidget * = new Urho3dWidget(context.Get(), this);
    w->Setup();
    w->Start();
}

void MainWindow::_addWindowTab(QWidget * widgetPtr, const std::string& name) {
    widgetPtr->resize(1280, 720);
    QGridLayout* layout = new QGridLayout;
    QTabWidget* newTab = new QTabWidget;
    int index = ui->tabWidget->addTab(newTab, name.c_str());
    ui->tabWidget->setTabEnabled(index, true);
    newTab->setLayout(layout);
    layout->addWidget(widgetPtr, 0, 0);
    newTab->show();
    widgetPtr->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

