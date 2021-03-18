#pragma once

#include "resource.h"
#include "../generic/engine_widget_internal.h"

class Tests;
class MainComponent;

class WickedWidgetInternal : EngineWidgetInternal {
public:
	WickedWidgetInternal(QWidget* parent);
	~WickedWidgetInternal();
	void exposeEvent(bool visible = true);
	void resizeEvent(QResizeEvent*) override;
	void runThread();
protected:
	//Tests * tests;
	MainComponent* mainComponent;
	bool quit = false;
	SDL_Window* window;
};

class WickedWidgetInternalWindows : public WickedWidgetInternal {
public:
	WickedWidgetInternalWindows(QWidget* parent);
};

class WickedWidgetInternalSDL : public WickedWidgetInternal, public SDLWidget {
public:
	WickedWidgetInternalSDL(QWidget* parent);
};