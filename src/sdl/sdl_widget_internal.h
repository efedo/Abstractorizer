#pragma once

#include "../generic/engine_widget_internal.h"

#include <QWidget>
#include <QTimer>
#include <SDL.h>

//#define FRAME_RATE 60
//#define MS_PER_FRAME 1000/FRAME_RATE

class SDLWidgetInternal : public EngineWidgetInternal
{
public :
    SDLWidgetInternal(QWidget* parent);
    virtual ~SDLWidgetInternal() override;
public slots:
    void SDLRepaint();
protected:
    SDL_Window* window;
    SDL_Renderer* renderer;
private:
    virtual void Init();
    virtual void Update();
    virtual void OnResize(int w, int h);

    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent*) override;
    void createWindow();
    QPaintEngine* paintEngine() const override;
    QTimer frameTimer;
    float msperframe = 1000.0f / 60.0f;
};
