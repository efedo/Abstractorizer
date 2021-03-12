#pragma once

#include "Abstractorizer/src/generic/engine_widget.h"
#include <iostream>

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