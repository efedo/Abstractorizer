#pragma once

#include "../generic/engine_widget_internal.h"

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <SDL.h>

class SDLWidgetInternal : public EngineWidgetInternal
{
public:
    SDLWidgetInternal();
    virtual ~SDLWidgetInternal() override;
    void initSDLvideo();
#ifdef USE_QT
public slots:
#endif // USE_QT
    void SDLRepaint();
protected:
    SDL_Window* sdlwindow;
    SDL_Renderer* renderer;
    QTimer frameTimer;
private:
    virtual void Init();
    virtual void Update();
    virtual void OnResize(int w, int h);

    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent*) override;
    void createWindow();
    QPaintEngine* paintEngine() const override;
    QTime timeClass;
    float msperframe = 1000.0f / 60.0f;
};