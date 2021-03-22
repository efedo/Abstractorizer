#include "ogrenext_widget.h"
#include "ogrenext_widget_internal.h"

OgreNextWidget::OgreNextWidget(QWidget* parent) : EngineWidget(parent, new OgreNextWidgetInternal, "OGRE-next") {}