#pragma once

#include <QWindow>
#include <QWidget>
#include "../generic/engine_widget.h"

// Provides a window with a Qt3D scene and a framegraph with a gui pass
class WickedWindow : public QWindow
{
public:
protected:
    void exposeEvent(QExposeEvent *) override;
    void resizeEvent(QResizeEvent *) override;
private:
};
