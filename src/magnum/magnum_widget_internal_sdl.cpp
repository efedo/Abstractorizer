#include "magnum_widget_internal_sdl.h"
#include <iostream>
#include <QWindow>
#include <QVBoxLayout>
#include <SDL.h>
#include <SDL_syswm.h>

int MagnumWidgetInternalSDL::dummyarg = 0;

MagnumWidgetInternalSDL::MagnumWidgetInternalSDL(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent), Magnum::Platform::Sdl2Application{ { dummyarg, (char**)(nullptr) } } //Configuration{}.setTitle("Magnum SDL Example")
{

    // https://stackoverflow.com/questions/33699258/qt-5-5-embed-external-application-into-qwidget

    initializeGfx();

    //Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
    //Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);


    SDL_Window* sdlwindow = this->Magnum::Platform::Sdl2Application::window();
    uint32_t sdl_window_id = SDL_GetWindowID(sdlwindow);

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(sdlwindow, &wmInfo);
    HWND native_window_id = wmInfo.info.win.window;
    std::cout << "Native SDL window id: " << native_window_id << "\n";

    QWindow* window = QWindow::fromWinId((WId)(native_window_id));
    window->setFlags(Qt::FramelessWindowHint);
    QWidget* widget = QWidget::createWindowContainer(window);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(widget);
    this->setLayout(layout);
}
//
//void MagnumWidgetInternalSDL::drawLoop() {
//    while (!stop_draw_loop) {
//        //frameTimer.restart();
//        drawEvent();
//        //qint64 ns = frameTimer.nsecsElapsed();
//    }
//}

void MagnumWidgetInternalSDL::drawEvent() {
    //if (!this->isVisible()) return;
    Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::ExitExternal);
    Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

    /* TODO: Add your drawing code here */
    _shader->draw(*_mesh);
    swapBuffers();
    Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::EnterExternal);
}

void MagnumWidgetInternalSDL::showEvent(QShowEvent*) {
    connect(&frameTimer, &QTimer::timeout, this, &MagnumWidgetInternalSDL::drawEvent);
    frameTimer.setInterval(this->msperframe);
    frameTimer.start();
    //drawEvent();
    //drawThread = new std::thread(&MagnumWidgetInternalSDL::drawEvent, this);
    //drawThread->join();
}

MagnumWidgetInternalSDL::~MagnumWidgetInternalSDL() {
    //if (drawThread) {
    //    stop_draw_loop = true;
    //    drawThread->join();
    //    delete drawThread;
    //    drawThread = 0;
    //}
}