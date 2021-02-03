#include "magnum_widget.h"
#include "magnum_widget_internal.h"
#include <QResizeEvent>

MagnumWidget::MagnumWidget(QWidget* parent)
	: EngineWidget(parent)
{
	internalWidgetPtr = new MagnumWidgetInternal(this);
}

MagnumWidget::~MagnumWidget() {
	if (internalWidgetPtr) {
		delete internalWidgetPtr;
		internalWidgetPtr = 0;
	}
}

void MagnumWidget::exposeEvent(bool visible)
{
}

void MagnumWidget::resizeEvent(QResizeEvent* event) 
{
	internalWidgetPtr->resize(event->size());
}