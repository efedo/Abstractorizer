#pragma once

#include "../generic/engine_widget.h"

class MagnumWidgetInternalOGL;
class MagnumWidgetInternalSDL;

using MagnumWidgetOGL = EngineWidgetWrapper<MagnumWidgetInternalOGL>;
using MagnumWidgetSDL = EngineWidgetWrapper<MagnumWidgetInternalSDL>;
//
//class MagnumWidget : public EngineWidget {
//public:
//	MagnumWidget(QWidget* parent);
//	~MagnumWidget();
//	void exposeEvent(bool visible = true) override;
//protected:
//	MagnumWidgetInternal * internalWidgetPtr = 0;
//};
//
//class MagnumWidgetOGL : public MagnumWidget {
//public:
//	MagnumWidgetOGL(QWidget* parent);
//	void resizeEvent(QResizeEvent*) override;
//};
//
//class MagnumWidgetSDL : public MagnumWidget {
//public:
//	MagnumWidgetSDL(QWidget* parent);
//};