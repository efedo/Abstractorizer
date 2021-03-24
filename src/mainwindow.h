// (c) 2021 Eric Fedosejevs and contributors. Licensed under MIT license (see LICENSE for details)

#pragma once

#include <QMainWindow>
#include <QGridLayout>
class EngineWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow* ptr;
private:
    template <class EngType, bool Relaunch>
    void _addWindowTab(const std::string & name);
    QTabWidget* tabWidget;
};