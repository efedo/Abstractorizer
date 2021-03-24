// (c) 2021 Eric Fedosejevs and contributors. Licensed under MIT license (see LICENSE for details)

#include <thread>

//#include "ui_mainwindow.h"
#include "mainwindow.h"

#include "backends/generic/engine_widget.h"
#include "backends/generic/engine_spawner.h"

#ifdef USE_QT3D
#include "backends/qt3d/qt3d_widget.h"
#endif

#ifdef USE_MAGNUM
#include "backends/magnum/magnum_widget.h"
#endif

#ifdef USE_SDL
#include "backends/sdl/sdl_widget.h"
#endif

#ifdef USE_WICKED
#include "backends/wicked/wicked_widget.h"
#endif

#ifdef USE_TINY_RENDERER
#include "backends/tinyrenderer/tinyrenderer_widget.h"
#endif

#ifdef USE_LVGL
#include "guis/lvgl/simulator.h"
#endif

#include <QLabel>
#include <QProcess>

MainWindow* MainWindow::ptr = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Abstractorizer");
    tabWidget = new QTabWidget;
    this->setCentralWidget(tabWidget);

    if (ptr) throw;
    ptr = this;

    #ifdef USE_QT3D
    _addWindowTab<Qt3DWidget, false>("Qt3D");
    #endif

    #ifdef USE_SDL
    _addWindowTab<SDLWidget, true>("SDL");
    #endif

    #ifdef USE_MAGNUM
    _addWindowTab<MagnumWidget, true>("Magnum");
    #endif

    #ifdef USE_WICKED
    _addWindowTab<WickedWidgetWindows, false>("Wicked");
    #endif

    #ifdef USE_TINY_RENDERER
    _addWindowTab<TinyRendererWidget, true>("TinyRenderer");
    #endif

    #ifdef USE_LVGL
    //lvglmain();
    #endif
}

class EngineTabBase : public QWidget {
public:
    std::unique_ptr<EngineSpawnerBase> spawner;
    std::unique_ptr<EngineWidget> engine;
    const std::string name;
    QGridLayout mylayout;
    QLabel placeholderlabel;
    uint32_t numplaceholders = 0;

    EngineTabBase(const std::string newname) : name(newname) {
        this->setObjectName(QString::fromStdString(name));
        this->setLayout(&mylayout);
        showplaceholder();
    }

    void showplaceholder() {
        ++numplaceholders;
        std::string labeltext = "Placeholder widget for " + name + " (created " + std::to_string(numplaceholders) + " times)";
        placeholderlabel.setText(QString::fromStdString(labeltext));
        mylayout.addWidget(&placeholderlabel);
        placeholderlabel.show();
    }

    void hideEvent(QHideEvent* hideevent) override {
        hide();
        QWidget::hideEvent(hideevent);
    }

    void hide() {
        if (engine) {
            engine.reset();
            mylayout.removeWidget(engine.get());
            showplaceholder();
        }
        else {
            std::cerr << "Error: engine " << name << " not running?\n";
        }
        std::cout << "Hiding engine tab: " << name << "\n";
    }

    void showEvent(QShowEvent* showevent) override {       
        show();
        QWidget::showEvent(showevent);

        // Reset (in vain on Windows) the main window as the active window
        // after a call to createWindowContainer
        MainWindow::ptr->setWindowState(Qt::WindowActive);
        this->setFocus();

        // Maximizing the window seems to reliably grab focus
        MainWindow::ptr->showMaximized();
        MainWindow::ptr->showNormal();

        // Try (in vain on Windows) to get the main window to repaint
        // (needed to cover up artifacts after window containerization)
        MainWindow::ptr->update(); // Doesn't actually repaint (at least on Windows)
        MainWindow::ptr->repaint(); // Doesn't actually repaint???? (at least on Windows)

        // Changing the window size seems to lead to a reliable repaint
        int oldw = MainWindow::ptr->size().width();
        int oldh = MainWindow::ptr->size().height();
        MainWindow::ptr->resize(oldw + 1, oldh + 1);
        MainWindow::ptr->resize(oldw, oldh);
    }

    //QLabel blah;

    void show() {
        if (spawner) {
            if (!engine) {
                if (!spawner->alreadyLaunched || spawner->allowRelaunch()) {
                    engine.swap(spawner->spawn(this));     
                    // The order in which these QLayout changes are made is critical
                    // for some bizarre reason
                    placeholderlabel.setText("Deleting placeholder");
                    mylayout.addWidget(engine.get());
                    placeholderlabel.hide(); // Qt is                   
                }
                else {
                    placeholderlabel.setText(QString::fromStdString("Cannot respawn engine " + name + " (can only be initialized once per application)"));
                    mylayout.addWidget(&placeholderlabel);
                }
            }
            else {
                std::cerr << "Error: engine " << name << " already running?\n";
            }
        }
        else {
            std::cerr << "Error: no spawner for engine " << name << "?\n";
        }

        std::cout << "Showing engine tab: " << name << "\n";
    }
};

template <class EngType, bool Relaunch>
class EngineTab : public EngineTabBase {
public:
    EngineTab(const std::string newname) : EngineTabBase(newname) {
        spawner = std::make_unique<EngineSpawner<EngType, Relaunch>>();
    }
};

template <class EngType, bool Relaunch>
void MainWindow::_addWindowTab(const std::string& name) {
    EngineTab<EngType, Relaunch>* newTab = new EngineTab<EngType, Relaunch>(name);
    int index = tabWidget->addTab(newTab, name.c_str());
    tabWidget->setTabEnabled(index, true);
}

MainWindow::~MainWindow()
{}

