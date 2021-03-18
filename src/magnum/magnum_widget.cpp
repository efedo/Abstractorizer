#include "magnum_widget.h"
#include "magnum_widget_internal_ogl.h"
#include "magnum_widget_internal_sdl.h"
#include <QResizeEvent>
//
//MagnumWidget::MagnumWidget(QWidget* parent)
//	: EngineWidget(parent, "Magnum")
//{
//	//internalWidgetPtr = new MagnumWidgetInternal(this);
//}
//
//MagnumWidgetOGL::MagnumWidgetOGL(QWidget* parent)
//	: MagnumWidget(parent)
//{
//	internalWidgetPtr = new MagnumWidgetInternalOGL(this);
//}
//
//MagnumWidgetSDL::MagnumWidgetSDL(QWidget* parent)
//	: MagnumWidget(parent)
//{
//	internalWidgetPtr = new MagnumWidgetInternalSDL(this);
//}
//
//MagnumWidget::~MagnumWidget() {
//	if (internalWidgetPtr) {
//		delete internalWidgetPtr;
//		internalWidgetPtr = 0;
//	}
//}
//
//void MagnumWidget::exposeEvent(bool visible)
//{
//}
//
//void MagnumWidgetOGL::resizeEvent(QResizeEvent* event)
//{
//	((MagnumWidgetInternalOGL *)(internalWidgetPtr))->resize(event->size());
//}