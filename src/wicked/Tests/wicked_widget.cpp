#include "wicked_widget.h"
//#include "wicked_widget_internal.h"
#include <QResizeEvent>

// Include engine headers:
#include "WickedEngine.h"
#include "wicked_window.h"

WickedWidget::WickedWidget(QWidget* parent)
	: EngineWidget(parent), windowPtr(new WickedWindow), widgetPtr(QWidget::createWindowContainer(windowPtr))
{
	widgetPtr->setParent(this);

	// Declare main component once per application:
	MainComponent main;
	
	// Assign window that you will render to:
	main.SetWindow(HWND(windowPtr->handle()));

	// Run the application:
	while (true) {
		main.Run();
	}
}

WickedWidget::~WickedWidget() {
	if (widgetPtr) {
		delete widgetPtr;
		widgetPtr = 0;
	}
}

void WickedWidget::exposeEvent(bool visible)
{
}

void WickedWidget::resizeEvent(QResizeEvent* event)
{
	widgetPtr->resize(event->size());
}