#ifndef URHO3D_WINDOW_H
#define URHO3D_WINDOW_H

#include "../generic/engine_widget.h"

class Urho3DWidgetInternal;

// Provides a window with a Qt3D scene and a framegraph with a gui pass
class Urho3DWidget : public EngineWidget
{
public:
    Urho3DWidget(QWidget* parent);
    ~Urho3DWidget();
protected:
    void exposeEvent(bool visible = true) override;
    void resizeEvent(QResizeEvent*) override;
    Urho3DWidgetInternal* internalWidgetPtr = 0;
};

#endif // URHO3D_WINDOW_H