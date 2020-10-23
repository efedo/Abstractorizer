#ifndef URHO3D_WINDOW_H
#define URHO3D_WINDOW_H

#include "../generic/engine_window.h"

// Provides a window with a Qt3D scene and a framegraph with a gui pass
class Urho3DWindow : public EngineWindow
{
public:
    Urho3DWindow();
protected:
    void exposeEvent(QExposeEvent*) override;
    void resizeEvent(QResizeEvent*) override;
};

#endif // URHO3D_WINDOW_H