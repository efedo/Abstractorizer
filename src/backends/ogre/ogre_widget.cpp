#include "ogre_widget.h"
#include "ogre_widget_internal.h"

OgreWidget::OgreWidget(QWidget* parent) : EngineWidget(parent, new OgreWidgetInternal, "OGRE") {}