#include "wicked_widget.h"
#include "wicked_widget_internal.h"

WickedWidget::WickedWidget(QWidget* parent) : EngineWidget(parent, new WickedWidgetInternal, "Wicked Engine") {}