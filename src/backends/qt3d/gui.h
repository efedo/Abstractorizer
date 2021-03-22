#pragma once

#include <imgui/imgui.h>

class Qt3DImguiManager;

namespace Qt3DCore {
class QEntity;
}

namespace Qt3DRender {
class QTexture2D;
}

class Gui
{
public:
    Gui(Qt3DImguiManager& manager);
    void frame(Qt3DCore::QEntity *rootEntity);

private:
    Qt3DImguiManager & m_manager;
    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    Qt3DRender::QTexture2D *m_customTexture = nullptr;
};