#include "magnum_widget.h"
#include "magnum_widget_internal.h"

MagnumWidget::MagnumWidget(QWidget* parent)
	: EngineWidget(parent)
{
	internalWidgetPtr = new MagnumWidgetInternal(parent);
}