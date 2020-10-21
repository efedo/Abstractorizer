// Original code from https://github.com/alpqr/imgui-qt3d

#ifndef QT3D_IMGUI_MANAGER_H
#define QT3D_IMGUI_MANAGER_H

#include <QObject>
#include <QSize>

namespace Qt3DCore {
class QTransform;
class QEntity;
class QNode;
}

namespace Qt3DExtras {
    class QPhongMaterial;
}

namespace Qt3DRender {
class QBuffer;
class QTexture2D;
class QMaterial;
class QLayer;
class QGeometryRenderer;
class QShaderProgram;
class QScissorTest;
class QParameter;
class QFilterKey;
class QDepthTest;
class QNoDepthMask;
class QBlendEquation;
class QBlendEquationArguments;
class QCullFace;
class QColorMask;
class QPickEvent;
}

class ImguiInputEventFilter;
class ImguiWindowQt3D;
class Gui;

class ImguiManagerQt3D : public QObject {
    Q_OBJECT
public:
    ImguiManagerQt3D(ImguiWindowQt3D & parentWindow, Qt3DCore::QEntity & rootEntity);
    ~ImguiManagerQt3D();

    void setupInputEventSource();

    struct OutputInfo {
        QSize size;
        qreal dpr;
        Qt3DRender::QLayer *guiTag;
        Qt3DRender::QLayer *activeGuiTag;
        Qt3DRender::QFilterKey *guiTechniqueFilterKey;
    };

    void pickTexSlot(Qt3DRender::QPickEvent* pickEvent);
    void createScene();
    OutputInfo getOutputInfo();
    void updateFrame();
    void releaseResources();

    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enabled);

    float scale() const { return m_scale; }
    void setScale(float scale);

private:
    struct CmdListEntry;
    void resizePool(CmdListEntry *e, int newSize);
    Qt3DRender::QMaterial *buildMaterial(Qt3DRender::QScissorTest **scissor);
    void updateGeometry(CmdListEntry *e, int idx, uint elemCount, int vertexCount, int indexCount, const void *indexOffset);
    void update3D();
    void updateInput();

    OutputInfo m_outputInfo;

    ImguiWindowQt3D & _parentWindow;
    Qt3DCore::QEntity & _rootEntity;
    Gui * _gui = nullptr;
    Qt3DExtras::QPhongMaterial* toggleTextMat = nullptr;

    ImguiInputEventFilter *m_inputEventFilter = nullptr;
    bool m_inputInitialized = false;
    bool m_enabled = true;
    float m_scale = 1.0f;

    Qt3DRender::QTexture2D *m_atlasTex = nullptr;
    struct SharedRenderPassData {
        bool valid = false;
        Qt3DRender::QShaderProgram *progES2;
        Qt3DRender::QShaderProgram *progGL3;
        Qt3DRender::QFilterKey *techniqueFilterKey;
        Qt3DRender::QDepthTest *depthTest;
        Qt3DRender::QNoDepthMask *noDepthWrite;
        Qt3DRender::QBlendEquation *blendFunc;
        Qt3DRender::QBlendEquationArguments *blendArgs;
        Qt3DRender::QCullFace *cullFace;
        Qt3DRender::QColorMask *colorMask;
        QVector<Qt3DCore::QNode *> enabledToggle;
    } rpd;

    struct CmdEntry {
        Qt3DCore::QEntity *entity = nullptr;
        Qt3DRender::QMaterial *material = nullptr;
        Qt3DRender::QGeometryRenderer *geomRenderer = nullptr;
        Qt3DCore::QTransform *transform = nullptr;
        Qt3DRender::QScissorTest *scissor = nullptr;
        Qt3DRender::QParameter *texParam = nullptr;
    };

    struct CmdListEntry {
        Qt3DRender::QBuffer *vbuf = nullptr;
        Qt3DRender::QBuffer *ibuf = nullptr;
        QVector<CmdEntry> cmds;
        int activeSize = 0;
    };

    QVector<CmdListEntry> m_cmdList;
};

#endif
