#include "qt3d_widget.h"
#include "qt3d_window.h"

#include <QResizeEvent>

Qt3DWidget::Qt3DWidget(QWidget* parent)
	: EngineWidget(parent), windowPtr(new Qt3DWindow), widgetPtr(QWidget::createWindowContainer(windowPtr))
{
	widgetPtr->setParent(this);
}

Qt3DWidget::~Qt3DWidget() {
	if (widgetPtr) {
		delete widgetPtr;
		widgetPtr = 0;
	}
}

void Qt3DWidget::exposeEvent(bool visible)
{
}

void Qt3DWidget::resizeEvent(QResizeEvent* event)
{
	widgetPtr->resize(event->size());
}