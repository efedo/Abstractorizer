#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <QWindow>

class EngineWindow : public QWindow {
public:
	void exposeEvent(QExposeEvent*) override {}
	void resizeEvent(QResizeEvent*) override {}
};

#endif