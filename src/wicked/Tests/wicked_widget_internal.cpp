
#include "magnum_widget_internal.h"

WickedWidgetInternal::WickedWidgetInternal(QWidget* parent, Qt::WindowFlags f) : 
    QOpenGLWidget{ parent, f }, _context(new Magnum::Platform::GLContext{ Magnum::NoCreate })
{
    //_mesh = new Magnum::GL::Mesh;
    //_shader = new Magnum::Shaders::VertexColor2D;
    //initializeTriangle();
}

void WickedWidgetInternal::initializeTriangle() {
    using namespace Magnum::Math::Literals;

    struct TriangleVertex {
        Magnum::Vector2 position;
        Magnum::Color3 color;
    };
    const TriangleVertex data[]{
        {{-0.5f, -0.5f}, 0xff0000_rgbf},    /* Left vertex, red color */
        {{ 0.5f, -0.5f}, 0x00ff00_rgbf},    /* Right vertex, green color */
        {{ 0.0f,  0.5f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
    };

    Magnum::GL::Buffer buffer;
    buffer.setData(data);

    _mesh->setCount(3)
        .addVertexBuffer(std::move(buffer), 0,
            Magnum::Shaders::VertexColor2D::Position{},
            Magnum::Shaders::VertexColor2D::Color3{});
}

void WickedWidgetInternal::initializeGL() {

    _context->create();

    /* TODO: Add your initialization code here */

    /* Clean up Magnum state when giving control back to Qt */
    Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::EnterExternal);
}

void WickedWidgetInternal::paintGL() {
    /* Reset state to avoid Qt affecting Magnum */
    Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::ExitExternal);

    /* Using framebuffer provided by Qt as default framebuffer */
    auto qtDefaultFramebuffer = Magnum::GL::Framebuffer::wrap(defaultFramebufferObject(), { {}, {width(), height()} });

    qtDefaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

    /* TODO: Add your drawing code here */
    //_shader->draw(*_mesh);
    //Magnum::swapBuffers();

    /* Clean up Magnum state when giving control back to Qt */
    Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::EnterExternal);
}