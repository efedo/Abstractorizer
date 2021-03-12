// Original code from https://github.com/alpqr/imgui-qt3d

#pragma once

#include <QWindow>
#include <QWidget>
#include "../generic/engine_widget.h"
#include <Qt3DCore/QAspectEngine>

namespace Qt3DRender {
class QCamera;
class QLayer;
class QFilterKey;
}

class Gui;
class Qt3DImguiManager;

// Provides a window with a Qt3D scene and a framegraph with a gui pass
class Qt3DWindow : public QWindow
{
public:
    Qt3DWindow();
protected:
    void exposeEvent(QExposeEvent *) override;
    void resizeEvent(QResizeEvent *) override;
private:
    friend class Qt3DImguiManager;
    void createAspectEngine();
    void createFramegraph();
    std::unique_ptr<Qt3DImguiManager> _guiMgr;
    std::unique_ptr<Qt3DCore::QAspectEngine> _aspectEngine;
    Qt3DCore::QEntity *m_rootEntity = nullptr;
    Qt3DRender::QCamera *m_sceneCamera = nullptr;
    Qt3DRender::QCamera *m_guiCamera = nullptr;
    Qt3DRender::QLayer *m_guiTag = nullptr;
    Qt3DRender::QLayer *m_activeGuiTag = nullptr;
    Qt3DRender::QFilterKey *m_guiTechniqueFilterKey = nullptr;
    Qt3DRender::QLayer* guiTag() const { return m_guiTag; }
    Qt3DRender::QLayer* activeGuiTag() const { return m_activeGuiTag; }
    Qt3DRender::QFilterKey* guiTechniqueFilterKey() const { return m_guiTechniqueFilterKey; }
};
