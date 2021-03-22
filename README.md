*** Currently in early testing!!! ***

Everyone and their dog is developing an open source 3D graphics/game engine these days. Some will last,
most won't. 

Meanwhile, desktop 3D capabilities in Qt have regressed with the unofficial abandonment of Qt3D.
So what do you do if you need to build a cross-platform desktop application with a 3D view and do not want to
roll you own OpenGL/DirectX/Vulkan/Metal code? Should you try to embed an existing engine and, if so, which one?

Abstractorizer is a project to compare existing graphics/game engines embedded in a Qt window, providing a 
scaffold for the development of cross-platform Qt applications. Where possible, it wraps engine-supplied 
windowing methods, otherwise it embeds the engine in an intermediate SDL widget. It does not use QML, QtQuick, 
or any other vendor lock-in, only standard C++ and LGPL QtWidgets. It runs on Qt 5.1 or greater, and 
should run on CopperSpice with minimal tinkering (not yet tested!).

Eventually, it may have a standard interface/test suite, allowing identical tests to be run in different engines,
and making it less painful to switch between them, or to gradually replace Qt in your application with an 
embedded GUI, such as ImGui.

Contributions are very welcome, especially if you would like to help speed inclusion of a specific engine!

---

To be considered for inclusion, a graphics/game engine should ideally:
- Be cross-platform
- Be useable as a library via a C++ API (either directly or with minimal tinkering)
- Provide basic features required for testing (e.g. font renderer, primitives library, code-based mesh construction)
- Have decent documentation
- Be either buildable or configurable via CMake
- Be under active development

Currently included:
- Magnum
- Qt3D
- SDL
- tinyrenderer (https://github.com/ssloy/tinyrenderer)

Queued for inclusion:
- zauonlok renderer (https://github.com/zauonlok/renderer)
- OGRE classic (i.e. OGRE 1)
- OGRE-next  (i.e. OGRE 2)
- BGFX
- Panda3D
- Quartz (https://github.com/Nadrin/Quartz)

Under consideration:
- Godot: if the editor dependency can be worked-around
- Filament
- Diligent Engine
- The Forge
- Yocto/GL

On hold:
- Wicked Engine: no primitives library, dynamic mesh construction, limited documentation

Rejected:
- QtQuick3D: no public C++ API
- Urho3D: limited documentation, CMake build is a mess, project may be dead?