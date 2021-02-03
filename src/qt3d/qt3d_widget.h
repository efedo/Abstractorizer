#ifndef QT3D_IMGUI_WIDGET_H
#define QT3D_IMGUI_WIDGET_H

#include "../generic/engine_widget.h"

class Qt3DWindow;

class Qt3DWidget : public EngineWidget {
public:
	Qt3DWidget(QWidget* parent);
	~Qt3DWidget();
	void exposeEvent(bool visible = true) override;
	void resizeEvent(QResizeEvent*) override;
private:
	Qt3DWindow * windowPtr;
	QWidget * widgetPtr;
};

#endif //QT3D_IMGUI_WIDGET_H