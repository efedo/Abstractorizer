//#pragma once

#include "engine_widget.h"

class EngineSpawnerBase {
public:
	virtual std::unique_ptr<EngineWidget> spawn(QWidget* parent) {
		std::cerr << "Should have been overriden\n";
		alreadyLaunched = true;
		return std::move(std::make_unique<EngineWidget>(parent, new EngineWidgetInternal, "generic"));
	}
	virtual bool allowRelaunch() { return false; }
	bool alreadyLaunched = false;
};

template <class EngType, bool Relaunch>
class EngineSpawner : public EngineSpawnerBase {
public:
	virtual std::unique_ptr<EngineWidget> spawn(QWidget* parent) override {
		assert(!alreadyLaunched || allowRelaunch());
		alreadyLaunched = true;
		return std::move(std::make_unique<EngType>(parent));
	}
	virtual bool allowRelaunch() { return Relaunch; }
};

