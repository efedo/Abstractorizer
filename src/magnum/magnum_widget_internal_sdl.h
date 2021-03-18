#pragma once
#include "magnum_widget_internal.h"
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <QTimer>
#include <QElapsedTimer>
#include <QWidget>
#include <thread>

class MagnumWidgetInternalSDL : public MagnumWidgetInternal, public Magnum::Platform::Sdl2Application, public QWidget{
public:
    explicit MagnumWidgetInternalSDL(QWidget* parent, Qt::WindowFlags f = nullptr);
    ~MagnumWidgetInternalSDL();
private:
    void drawLoop();
    std::thread * drawThread = 0;
    void drawEvent() override;
    void showEvent(QShowEvent*) override;
    static int dummyarg; // only here to provide a dummy lvalue for magnum sdl2application arguments
    QTimer frameTimer;
    float msperframe = 1000.0f / 60.0f;

    //void mousePressEvent(MouseEvent& event) override;
    //void mouseReleaseEvent(MouseEvent& event) override;
    //void mouseMoveEvent(MouseMoveEvent& event) override;
};