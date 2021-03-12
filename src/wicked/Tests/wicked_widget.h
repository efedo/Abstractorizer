#pragma once

#include "../generic/engine_widget.h"

class WickedWindow;

class WickedWidget : public EngineWidget {
public:
	WickedWidget(QWidget* parent);
	~WickedWidget();
	void exposeEvent(bool visible = true) override;
	void resizeEvent(QResizeEvent*) override;
private:
	WickedWindow* windowPtr;
	QWidget* widgetPtr;
};