#pragma once

#include "../generic/engine_widget.h"


class MagnumWidgetInternal;

class MagnumWidget : public EngineWidget {
public:
	MagnumWidget(QWidget* parent);
	~MagnumWidget();
	void exposeEvent(bool visible = true) override;
	void resizeEvent(QResizeEvent*) override;
private:
	MagnumWidgetInternal * internalWidgetPtr = 0;
};