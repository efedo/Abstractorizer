#include "sdl_widget.h"
#include "sdl_widget_internal.h"

SDLWidget::SDLWidget(QWidget* parent) : EngineWidget(parent, new SDLWidgetInternal, "SDL") {}