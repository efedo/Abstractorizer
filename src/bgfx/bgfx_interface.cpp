/*
 * The purpose of this file is to provide a neat bridge between the bgfx example framework
 * and user code, allowing the sample framework and whole bgfx library (and dependents)
 * to be pasted without modification into the lib directory. This is only necessary becase the bgfx sample
 * framework contains various utilities that would be rather tedious to rewrite, and yet the sample framework
 * would require considerable refactoring to build as an independent library.
 *
 * Copyright 2019 Eric Fedosejevs
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

 // Note that in what can only be described as extremely annoying, common.h somehow overrides MSVS's target path
 // so that the resultant .exe ends up named after this .cpp file in the base folder.
 // Currently leaving this behavior as is since I don't want to locally change example framework.

#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"
#include "main.h"

namespace
{


class ExampleCubes : public entry::AppI
{
public:

	ExampleCubes(const char* _name, const char* _description, const char* _url)
		: entry::AppI(_name, _description, _url)
	{}

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
	{
		cManager::init(_argc, _argv, _width, _height, this);
	}

	virtual int shutdown() override
	{
		return cManager::get()->shutdown();
	}

	bool update() override
	{
		return cManager::get()->update();
	}
};

} // namespace

ENTRY_IMPLEMENT_MAIN(
	  ExampleCubes
	, "01-cubes"
	, "Rendering simple static mesh."
	, "https://bkaradzic.github.io/bgfx/examples.html#cubes"
	);
