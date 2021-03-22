#include "qt3d_widget.h"
#include "qt3d_widget_internal.h"
#include "qt3d_window.h"

#include <QResizeEvent>

Qt3DWidgetInternal::Qt3DWidgetInternal()
	: windowPtr(new Qt3DWindow), widgetPtr(QWidget::createWindowContainer(windowPtr))
{
	widgetPtr->setParent(this);
}

Qt3DWidgetInternal::~Qt3DWidgetInternal() {
	//if (widgetPtr) {
	//	delete widgetPtr;
	//	widgetPtr = 0;
	//}
	//if (windowPtr) {
	//	delete windowPtr;
	//	windowPtr = 0;
	//}
}

void Qt3DWidgetInternal::exposeEvent(bool visible)
{
}

void Qt3DWidgetInternal::resizeEvent(QResizeEvent* event)
{
	widgetPtr->resize(event->size());
}