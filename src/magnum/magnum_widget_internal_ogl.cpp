#include "magnum_widget_internal_ogl.h"
#include <iostream>

MagnumWidgetInternalOGL::MagnumWidgetInternalOGL(QWidget* parent, Qt::WindowFlags f) :
    QOpenGLWidget{ parent, f }, _context(new Magnum::Platform::GLContext{ Magnum::NoCreate })
{}

void MagnumWidgetInternalOGL::initializeGL() {

    _context->create();

    /* TODO: Add your initialization code here */
    initializeGfx();

    /* Clean up Magnum state when giving control back to Qt */
    Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::EnterExternal);
}

void MagnumWidgetInternalOGL::paintGL() {
    /* Reset state to avoid Qt affecting Magnum */
    Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::ExitExternal);

    /* Using framebuffer provided by Qt as default framebuffer */
    auto qtDefaultFramebuffer = Magnum::GL::Framebuffer::wrap(defaultFramebufferObject(), { {}, {width(), height()} });

    qtDefaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

    /* TODO: Add your drawing code here */
    _shader->draw(*_mesh);
    //swapBuffers();

    /* Clean up Magnum state when giving control back to Qt */
    Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::EnterExternal);
}