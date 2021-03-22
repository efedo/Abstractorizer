#include "tinyrenderer_widget.h"
#include "tinyrenderer_widget_internal.h"

TinyRendererWidget::TinyRendererWidget(QWidget* parent) : EngineWidget(parent, new TinyRendererWidgetInternal, "TinyRenderer") {}