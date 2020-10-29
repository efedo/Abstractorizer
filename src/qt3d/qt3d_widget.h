#ifndef QT3D_IMGUI_WIDGET_H
#define QT3D_IMGUI_WIDGET_H

#include "../generic/engine_widget.h"

class Qt3DWindow;

class Qt3DWidget : public EngineWidget {
public:
	Qt3DWidget();
private:
	Qt3DWindow * windowPtr;
};

#endif //QT3D_IMGUI_WIDGET_H