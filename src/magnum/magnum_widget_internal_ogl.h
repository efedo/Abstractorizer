#pragma once
#include "magnum_widget_internal.h"

class MagnumWidgetInternalOGL : public MagnumWidgetInternal, public QOpenGLWidget {
public:
    explicit MagnumWidgetInternalOGL(QWidget* parent, Qt::WindowFlags f = nullptr);
protected:
    void initializeGL() override;
    void paintGL() override;
private:
    Magnum::Platform::GLContext* _context;
};