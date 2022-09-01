#pragma once

#include "../generic/engine_widget_internal.h"
#include "../sdl/sdl_widget_internal.h"

#include <atomic>
#include <thread>
#include <mutex>

namespace TinyScene {
	class Model;
	class Shader;
}

class ZauonlokRendererWidgetInternal : public EngineWidgetInternal
{
public:
	ZauonlokRendererWidgetInternal();
private:
};
