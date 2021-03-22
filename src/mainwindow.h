#pragma once

#include <QMainWindow>
#include <QGridLayout>
class EngineWidget;

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

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
    //QGridLayout* gridWidget;
    QTabWidget* tabWidget;
    //Ui::MainWindow *ui;
};