#pragma once

#include "../generic/engine_widget.h"
#include <iostream>

class Qt3DWindow;
class QWidget;

class Qt3DWidgetInternal : public EngineWidgetInternal {
public:
	Qt3DWidgetInternal();
	~Qt3DWidgetInternal();
	void exposeEvent(bool visible = true) override;
	void resizeEvent(QResizeEvent*) override;
private:
	Qt3DWindow * windowPtr;
	QWidget * widgetPtr;
};