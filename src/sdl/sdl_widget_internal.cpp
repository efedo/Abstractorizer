#include <iostream>
#include <QWindow>
#include <QVBoxLayout>
#include <SDL.h>
#include <SDL_syswm.h>
#include "sdl_widget_internal.h"

SDLWidgetInternal::SDLWidgetInternal(QWidget* parent) {
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw "SDL_Error: " + std::string(SDL_GetError());
    }

    printf("Creating SDL window\n");

    //window = SDL_CreateWindowFrom(reinterpret_cast<void*>(winId()));
    //if(window == NULL)
    //    throw "Can't create window: " + std::string(SDL_GetError());
    window = SDL_CreateWindow("SDL App",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_OPENGL); // SDL_WINDOW_VULKAN
    if (window == NULL) {
        std::cerr << "SDL error: " << std::string(SDL_GetError()) << "\n";
        throw "Can't create window: " + std::string(SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
        throw "Can't create renderer: " + std::string(SDL_GetError());

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
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

void SDLWidgetInternal::resizeEvent(QResizeEvent*) {
    int x = static_cast<int>(QWidget::width());
    int y = static_cast<int>(QWidget::height());
    OnResize(x, y);
}

void SDLWidgetInternal::showEvent(QShowEvent*) {
    Init();
    /* frameTimer will send signal timeout() every 60th of a second, connect to "repaint" */
    connect(&frameTimer, &QTimer::timeout, this, &SDLWidgetInternal::SDLRepaint);
    frameTimer.setInterval(this->msperframe);
    frameTimer.start();
}

void SDLWidgetInternal::SDLRepaint() {
    //if (!this->isVisible()) return;
    Update();
    SDL_RenderPresent(renderer);
}

SDLWidgetInternal::~SDLWidgetInternal() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

/* Override default system paint engine to remove flickering */
QPaintEngine* SDLWidgetInternal::paintEngine() const {
    return reinterpret_cast<QPaintEngine*>(0);
}

void SDLWidgetInternal::Init() {
    printf("example init called\n");
    SDLRepaint();
}

void SDLWidgetInternal::Update() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, new SDL_Rect{ 50,50,100,100 });
}

void SDLWidgetInternal::OnResize(int, int) {
    printf("example resize called\n");
    SDLRepaint();
}
