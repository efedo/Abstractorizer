#include "zauonlokrenderer_widget.h"
#include "zauonlokrenderer_widget_internal.h"
#include <iostream>

ZauonlokRendererWidget::ZauonlokRendererWidget(QWidget* parent) : EngineWidget(parent, new ZauonlokRendererWidgetInternal, "ZauonlokRenderer") 
{}