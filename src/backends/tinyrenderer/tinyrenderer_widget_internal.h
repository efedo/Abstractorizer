#pragma once

#include "../generic/engine_widget_internal.h"
#include "../sdl/sdl_widget_internal.h"

#include <atomic>
#include <thread>
#include <mutex>
#include "argb.h"

class Model;
class Shader;

class TinyRendererWidgetInternal : public SDLWidgetInternal
{
public:
	TinyRendererWidgetInternal();
	virtual ~TinyRendererWidgetInternal() override;
	virtual void Update() override;
	void render();
private:
	Uint64 start = 0;
	Model * model = 0;
	Shader * shader = 0;
	SDL_Texture * texture = 0;
	SDL_Surface * surface = 0;
	std::thread renderthread;
	std::mutex texturemutex;
	std::vector<double> zbuffer; // note that the z-buffer is initialized with minimal possible values
	void setupView();
	uint16_t width = 800;
	uint16_t height = 600;

	std::atomic<bool> stoprendering = false;

	//TGAImage * framebuffer; // the output image
	//const int width = 800; // output image size
	//const int height = 800;
};
