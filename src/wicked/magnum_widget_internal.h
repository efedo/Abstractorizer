#ifndef MAGNUM_INTERNAL_WIDGET_H
#define MAGNUM_INTERNAL_WIDGET_H

#include <Corrade/Containers/Optional.h>
/* Magnum GL headers always have to be included before Qt ones */
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
//#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/VertexColor.h>

/* If your application is using anything from QtGui, you might get warnings
   about GLEW and errors regarding GLclampf. If that's the case, uncomment the
   following and place it as early as possible (but again *after* including
   Magnum GL headers) */
//typedef GLfloat GLclampf;
//#undef __glew_h__ /* shh, Qt, shh */
//#undef __GLEW_H__
//#include <QtGui/qopenglfunctions.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QOpenGLWidget>

//namespace Magnum {
//    namespace GL {
//        class Mesh;
//    }
//    namespace Platform {
//        class GLContext;
//    }
//    namespace Shaders {
//        class VertexColor2D;
//    }
//}

class MagnumWidgetInternal : public QOpenGLWidget {
public:
    explicit MagnumWidgetInternal(QWidget* parent, Qt::WindowFlags f = nullptr);

private:
    void initializeTriangle();
    void initializeGL() override;
    void paintGL() override;

    Magnum::Platform::GLContext * _context;
    Magnum::GL::Mesh* _mesh;
    Magnum::Shaders::VertexColor2D* _shader;
};

#endif //MAGNUM_INTERNAL_WIDGET_H