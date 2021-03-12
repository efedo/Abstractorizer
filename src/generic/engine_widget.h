#pragma once

#include <QWidget>

class EngineWidget : public QWidget {
public:
	EngineWidget(QWidget* parent) : QWidget(parent) {}
	virtual void exposeEvent(bool visible = true) {}
	virtual void resizeEvent(QResizeEvent*) override {}
	//virtual void update() override {}
};
