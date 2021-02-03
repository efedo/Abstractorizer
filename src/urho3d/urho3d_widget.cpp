#include "urho3d_widget.h"
#include "urho3d_widget_internal.h"

#include <QResizeEvent>

Urho3DWidget::Urho3DWidget(QWidget* parent)
	: EngineWidget(parent)
{
	Urho3DWidgetInternal* internalWidgetPtr = 0;
}

Urho3DWidget::~Urho3DWidget() {
	if (internalWidgetPtr) {
		delete internalWidgetPtr;
		internalWidgetPtr = 0;
	}
}

void Urho3DWidget::exposeEvent(bool visible = true) {

}

void Urho3DWidget::resizeEvent(QResizeEvent* event) 
{
	internalWidgetPtr->resize(event->size());
}