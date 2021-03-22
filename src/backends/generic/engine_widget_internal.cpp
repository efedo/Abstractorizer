#include "engine_widget_internal.h"
#include "engine_widget.h"

//EngineWidgetInternal::EngineWidgetInternal() : QWidget() {}

void EngineWidgetInternal::exposeEvent(bool visible) {
	//QWidget::exposeEvent(visible);
};

void EngineWidgetInternal::resizeEvent(QResizeEvent* resizeEvent) {
	QWidget::resizeEvent(resizeEvent);
}