#pragma once

#include <QWidget>

class EngineWidget;

class EngineWidgetInternal : public QWidget {
public:
	//EngineWidgetInternal(); //EngineWidget* parent
	virtual void exposeEvent(bool visible = true);
	virtual void resizeEvent(QResizeEvent* resizeEvent);
};