#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class EngineWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    template <class EngType>
    void _addWindowTab(const std::string & name);
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
