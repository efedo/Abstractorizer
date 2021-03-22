#pragma once

#include "../generic/engine_widget_internal.h"
#include "stdafx.h"
#include "resource.h"

class WickedWidgetInternal : public EngineWidgetInternal {
public:
	WickedWidgetInternal();
	~WickedWidgetInternal();
	void exposeEvent(bool visible = true);
	void resizeEvent(QResizeEvent*) override;
	void runThread();
protected:
	//Tests * tests;
	MainComponent* mainComponent;
	bool quit = false;
	//SDL_Window* window;
};

class WickedWidgetInternalWindows : public WickedWidgetInternal {
public:
	WickedWidgetInternalWindows();
};

//class WickedWidgetInternalSDL : public WickedWidgetInternal, public SDLWidget {
//public:
//	WickedWidgetInternalSDL(QWidget* parent);
//};