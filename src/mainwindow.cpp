#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "generic/engine_widget.h"

#ifdef USE_QT3D
#include "qt3d/qt3d_widget.h"
#endif

#ifdef USE_MAGNUM
#include "magnum/magnum_widget.h"
#endif

//#include "urho3d/urho3d_widget.h"

#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    #ifdef USE_QT3D
    _addWindowTab<Qt3DWidget>("Qt3D");
    #endif

    #ifdef USE_MAGNUM
    _addWindowTab<MagnumWidget>("Magnum");
    #endif

    //_addWindowTab(new Urho3DWidgetInternal, "Urho3D");

    //Urho3D::SharedPtr<Urho3D::Context> context = new Urho3D::Context();
    //Urho3DWidgetInternal * = new Urho3DWidgetInternal(context.Get(), this);
    //w->Setup();
    //w->Start();
}

template <class EngType>
void MainWindow::_addWindowTab(const std::string& name) {
    QGridLayout* layout = new QGridLayout;
    QTabWidget* newTab = new QTabWidget;
    int index = ui->tabWidget->addTab(newTab, name.c_str());
    ui->tabWidget->setTabEnabled(index, true);
    newTab->setLayout(layout);
    EngineWidget* widgetPtr = new EngType(newTab);
    widgetPtr->resize(1280, 720);
    layout->addWidget(widgetPtr, 0, 0);
    newTab->show();
    widgetPtr->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

