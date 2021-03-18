#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "generic/engine_widget.h"
#include "generic/engine_spawner.h"

#ifdef USE_QT3D
#include "qt3d/qt3d_widget.h"
#endif

#ifdef USE_MAGNUM
#include "magnum/magnum_widget.h"
#endif

#ifdef USE_SDL
#include "sdl/sdlwidget.h"
#endif

#ifdef USE_WICKED
#include "wicked/wicked_widget.h"
#endif

#include <QLabel>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    #ifdef USE_QT3D
    _addWindowTab<Qt3DWidget, false>("Qt3D");
    #endif

    #ifdef USE_SDL
    _addWindowTab<SDLWidget, true>("SDL");
    #endif

    #ifdef USE_MAGNUM
    #ifdef USE_SDL
    _addWindowTab<MagnumWidgetSDL, true>("Magnum/SDL");
    #else
    _addWindowTab<MagnumWidgetOGL>("Magnum/OGL");
    #endif // USE_SDL
    #endif

    #ifdef USE_WICKED
    _addWindowTab<WickedWidgetWindows, false>("Wicked/Native");
    #endif

    //#ifdef USE_UNREAL
    //QString program = "C:/Users/ericf/Documents/Unreal Projects/NewTestGame/Saved/StagedBuilds/WindowsNoEditor/NewTestGame/Binaries/Win64/NewTestGame.exe";
    //QStringList arguments;
    //QProcess* myProcess = new QProcess(this);
    //myProcess->start(program, arguments);
    //#endif

    //Urho3D::SharedPtr<Urho3D::Context> context = new Urho3D::Context();
    //Urho3DWidgetInternal * = new Urho3DWidgetInternal(context.Get(), this);
    //w->Setup();
    //w->Start();
}

class EngineTabBase : public QTabWidget {
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
    }

    void hideEvent(QHideEvent * hideevent) override {
        if (engine) {
            engine.reset();
            mylayout.removeWidget(engine.get());
            showplaceholder();
        }
        else {
            std::cerr << "Error: engine " << name << " not running?\n";
        }
        QTabWidget::hideEvent(hideevent);
        std::cout << "Hiding engine tab: " << name << "\n";
    }

    void showEvent(QShowEvent* showevent) override {
        if (spawner) {
            if (!engine) {
                if (!spawner->alreadyLaunched || spawner->allowRelaunch()) {
                    engine.swap(spawner->spawn(this));
                    placeholderlabel.setText("Deleting placeholder");
                    mylayout.removeWidget(&placeholderlabel);
                    mylayout.addWidget(engine.get());
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
        QTabWidget::showEvent(showevent);
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
    //QGridLayout* layout = new QGridLayout;
    EngineTab<EngType, Relaunch>* newTab = new EngineTab<EngType, Relaunch>(name);
    int index = ui->tabWidget->addTab(newTab, name.c_str());
    ui->tabWidget->setTabEnabled(index, true);
    //newTab->setLayout(layout);
    //EngType* widgetPtr = new EngType(newTab);
    //widgetPtr->resize(1280, 720);
    //layout->addWidget(widgetPtr, 0, 0);
    //newTab->show();
    //widgetPtr->show();
    //return newTab->engine;
}

MainWindow::~MainWindow()
{
    delete ui;
}

