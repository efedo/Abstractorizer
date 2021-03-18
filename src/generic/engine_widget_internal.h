#pragma once

#include <QWidget>

class EngineWidgetInternal : public QWidget {
	virtual void exposeEvent(bool visible = true);
	virtual void resizeEvent(QResizeEvent* resizeEvent);
};