// (c) 2021 Eric Fedosejevs and contributors. Licensed under MIT license (see LICENSE for details)

#pragma once

#include <QMainWindow>
#include <QGridLayout>
class EngineWidget;

class MainWindow : public QMainWindow
{
    #ifdef USE_QT
    Q_OBJECT
    #else
    CS_OBJECT(MainWindow)
    #endif // USE_QT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow* ptr;
private:
    template <class EngType, bool Relaunch>
    void _addWindowTab(const std::string & name);
    QTabWidget* tabWidget;
};