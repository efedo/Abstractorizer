#include <iostream>

#include "magnum_widget_internal.h"

MagnumWidgetInternal::MagnumWidgetInternal() {

}


bool isInside(Magnum::Vector3 vec) {
    return true;
}

void MagnumWidgetInternal::addCube(Magnum::Vector3 lowpos, Magnum::Color3 color, float size) {
    //vertices.push_back(TriangleVertex{ lowpos + Magnum::Vector3{ 0.0f, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f} });
    //vertices.push_back(TriangleVertex{ lowpos + Magnum::Vector3{ 0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f} });
    //vertices.push_back(TriangleVertex{ lowpos + Magnum::Vector3{ 0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f} });
}

// external boundary


void MagnumWidgetInternal::initializeTriangle() {

    _mesh = std::make_unique<Magnum::GL::Mesh>();
    _shader = std::make_unique<Magnum::Shaders::VertexColor3D>(); //Shaders::Phong
    _buffer = std::make_unique<Magnum::GL::Buffer>();

    //_mesh = new Magnum::GL::Mesh;
    *_mesh = Magnum::MeshTools::compile(Magnum::Primitives::cubeSolid());

    //using namespace Magnum::Math::Literals;

    ////const TriangleVertex data[]{
    ////    {{-0.5f, -0.5f}, 0xff0000_rgbf},    /* Left vertex, red color */
    ////    {{ 0.5f, -0.5f}, 0x00ff00_rgbf},    /* Right vertex, green color */
    ////    {{ 0.0f,  0.5f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
    ////};

    //std::array<TriangleVertex, 6> myTriangles =
    //{
    //    TriangleVertex{{-0.5f, -0.5f,  0.0f}, 0xff0000_rgbf},    /* Left vertex, red color */
    //    TriangleVertex{{ 0.5f, -0.5f,  0.1f}, 0x00ff00_rgbf},    /* Right vertex, green color */
    //    TriangleVertex{{ 0.0f,  0.5f,  0.0f}, 0x0000ff_rgbf},     /* Top vertex, blue color */
    //    TriangleVertex{{ 0.0f, -0.5f,  0.2f}, 0x0000ff_rgbf},    /* Left vertex, red color */
    //    TriangleVertex{{ 1.0f, -0.5f,  0.0f}, 0x0000ff_rgbf},    /* Right vertex, green color */
    //    TriangleVertex{{ 0.5f,  0.5f,  0.0f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
    //};

    //Magnum::GL::Buffer buffer;
    //buffer.setData(myTriangles); //data

    //_mesh->setCount(6)
    //    .addVertexBuffer(std::move(buffer), 0,
    //        Magnum::Shaders::VertexColor3D::Position{},
    //        Magnum::Shaders::VertexColor3D::Color3{});
}

void MagnumWidgetInternal::initializeGfx() {
    initializeTriangle();
    std::cout << "Magnum graphics initialized?\n";
    *_shader = Magnum::Shaders::VertexColor3D();
}
