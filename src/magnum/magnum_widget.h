#ifndef MAGNUM_WIDGET_H
#define MAGNUM_WIDGET_H

#include "../generic/engine_widget.h"

class MagnumWidgetInternal;

class MagnumWidget : public EngineWidget {
public:
	MagnumWidget(QWidget* parent);
	void exposeEvent(bool visible = true) override {}
	void resizeEvent(QResizeEvent*) override {}
private:
	MagnumWidgetInternal * internalWidgetPtr = 0;
};

#endif //MAGNUM_WIDGET_H