#pragma once

class Tests;
class MainComponent;

class Blah {
};

#include "../generic/engine_widget.h"

#include <QWidget>
#include <QTimer>
#include <SDL.h>

class SDLWidgetInternal : public EngineWidget {
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
