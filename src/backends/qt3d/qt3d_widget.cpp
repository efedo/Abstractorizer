#include "qt3d_widget.h"
#include "qt3d_widget_internal.h"

Qt3DWidget::Qt3DWidget(QWidget* parent) : EngineWidget(parent, new Qt3DWidgetInternal, "Qt3D") {}
