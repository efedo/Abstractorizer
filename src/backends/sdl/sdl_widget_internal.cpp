#include <iostream>
#include <QWindow>
#include <QVBoxLayout>
#include <SDL.h>
#include <SDL_syswm.h>
#include "sdl_widget_internal.h"
#include "Utilogeny/source/core/exceptions.h"
#include "Utilogeny/source/core/platform.h"

SDLWidgetInternal::SDLWidgetInternal() {

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw "SDL_Error: " + std::string(SDL_GetError());
    }

    printf("Creating SDL window\n");

    // CopperSpice throws if you create an SDL window
    sdlwindow = SDL_CreateWindow("SDL App",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_HIDDEN); // SDL_WINDOW_VULKAN
    if (sdlwindow == NULL) {
        std::cerr << "SDL error: " << std::string(SDL_GetError()) << "\n";
        throw "Can't create window: " + std::string(SDL_GetError());
    }

    renderer = SDL_CreateRenderer(sdlwindow, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        throw "Can't create renderer: " + std::string(SDL_GetError());

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    std::cout << "SDL renderer name: " << info.name << std::endl;
    std::cout << "SDL texture formats: " << std::endl;
    for (Uint32 i = 0; i < info.num_texture_formats; i++)
    {
        std::cout << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
    }

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(sdlwindow, &wmInfo);

    #ifdef OS_WINDOWS
    auto native_window_id = wmInfo.info.win.window;
    #elif OS_MACOS
    auto native_window_id = wmInfo.info.cocoa.window;
    #elif OS_LINUX
    auto native_window_id = wmInfo.info.x11.window
    #endif

    std::cout << "Native SDL window id: " << native_window_id << "\n";

    QWindow* qwindow = QWindow::fromWinId((WId)(native_window_id));
    if (!qwindow) throwl("Failed to create QWindow from native SDL window");
    //window->setWindowState(Qt::WindowMinimized);
    ////window->showMinimized();

    qwindow->setFlags(Qt::FramelessWindowHint);

    QWidget* widget = QWidget::createWindowContainer(qwindow);
    //widget->setParent(this);
    //this->hide();
    QVBoxLayout* layout = new QVBoxLayout;
    this->setLayout(layout);
    layout->setMargin(0);
    layout->addWidget(widget);
}

void SDLWidgetInternal::initSDLvideo() {

    std::cout << "Initializing \"sdl\".";
    std::cout << "Available drivers:";
    int num_drivers = SDL_GetNumVideoDrivers();
    for (int i = 0; i < num_drivers; ++i) {
        std::cout << "\t" << SDL_GetVideoDriver(i);
    }

    /* Initialize defaults and Video subsystem */
    if (SDL_VideoInit(NULL) != 0) {
        throwl("Unable to initialize SDL video");
    }

    SDL_version version;
    SDL_GetVersion(&version);
    std::cout << "Initialized with SDL "
        << static_cast<int>(version.major) << '.'
        << static_cast<int>(version.minor) << '.'
        << static_cast<int>(version.patch)
        << " (driver: " << SDL_GetCurrentVideoDriver() << ")";


    // init window here
    // init renderer
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
    Update();
}

SDLWidgetInternal::~SDLWidgetInternal() {
    frameTimer.stop();
    disconnect(&frameTimer, &QTimer::timeout, this, &SDLWidgetInternal::SDLRepaint);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(sdlwindow);
}

/* Override default system paint engine to remove flickering */
QPaintEngine* SDLWidgetInternal::paintEngine() const {
    return reinterpret_cast<QPaintEngine*>(0);
}

void SDLWidgetInternal::Init() {
    SDLRepaint();
}

void SDLWidgetInternal::Update() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    int shift = int(float(timeClass.currentTime().msecsSinceStartOfDay() % 1000) / 10.0f);
    SDL_Rect myrect{ 50 + shift, 50 + shift, 100, 100 };
    SDL_RenderFillRect(renderer, &myrect);
    SDL_RenderPresent(renderer);
}

void SDLWidgetInternal::OnResize(int, int) {
    SDLRepaint();
}