// Original code from https://github.com/alpqr/imgui-qt3d

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DInput/QInputAspect>
#include <Qt3DAnimation/QAnimationAspect>
#include <Qt3DLogic/QLogicAspect>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DInput/QInputSettings>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DRender/QViewport>
#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QTechniqueFilter>
#include <Qt3DRender/QFilterKey>
#include <Qt3DRender/QNoDraw>
#include <Qt3DRender/QLayerFilter>
#include <Qt3DRender/QLayer>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QSortPolicy>
#include <QSurfaceFormat>
#include <QOpenGLContext>

#include "qt3d_window.h"
#include "qt3d_imgui_manager.h"

Qt3DWindow::Qt3DWindow()
{
    setSurfaceType(QSurface::OpenGLSurface);
    createAspectEngine();

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        fmt.setVersion(3, 2);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(fmt);

    setFormat(fmt);
    createFramegraph();

    _guiMgr = std::make_unique<Qt3DImguiManager>(*this, *m_rootEntity);
    _guiMgr->createScene(); // Might have to move later
}

void Qt3DWindow::createAspectEngine()
{
    _aspectEngine.reset(new Qt3DCore::QAspectEngine);
    //Qt3DRender::QRenderAspect * naspect = new Qt3DRender::QRenderAspect(Qt3DRender::QRenderAspect::RenderType::Threaded);
    Qt3DRender::QRenderAspect * naspect = new Qt3DRender::QRenderAspect;
    _aspectEngine->registerAspect(naspect);
    _aspectEngine->registerAspect(new Qt3DInput::QInputAspect);
    _aspectEngine->registerAspect(new Qt3DAnimation::QAnimationAspect);
    _aspectEngine->registerAspect(new Qt3DLogic::QLogicAspect);
}

void Qt3DWindow::exposeEvent(QExposeEvent *)
{
    static bool isRootSet = false;
    if (!m_rootEntity) createFramegraph();
    if (!isRootSet && isExposed()) {
        _aspectEngine->setRootEntity(Qt3DCore::QEntityPtr(m_rootEntity));
        isRootSet = true;
    }
}

void Qt3DWindow::resizeEvent(QResizeEvent *)
{
    if (m_sceneCamera)
        m_sceneCamera->setAspectRatio(width() / float(height()));

    if (m_guiCamera) {
        m_guiCamera->setRight(width() * devicePixelRatio());
        m_guiCamera->setBottom(height() * devicePixelRatio());
    }
}

void Qt3DWindow::createFramegraph()
{
    m_rootEntity = new Qt3DCore::QEntity;
    m_rootEntity->setObjectName(QLatin1String("root"));

    Qt3DRender::QRenderSettings *frameGraphComponent = new Qt3DRender::QRenderSettings(m_rootEntity);

    // RenderSurfaceSelector -> Viewport -> (ClearBuffers -> NoDraw) | (CameraSelector -> TechniqueFilter -> LayerFilter) | (CameraSelector -> TechniqueFilter -> SortPolicy -> LayerFilter)

    Qt3DRender::QRenderSurfaceSelector *targetSel = new Qt3DRender::QRenderSurfaceSelector;

    Qt3DRender::QViewport *viewport = new Qt3DRender::QViewport(targetSel);
    viewport->setNormalizedRect(QRectF(0, 0, 1, 1));

    Qt3DRender::QClearBuffers *clearBuffers = new Qt3DRender::QClearBuffers(viewport);
    clearBuffers->setBuffers(Qt3DRender::QClearBuffers::ColorDepthBuffer);
    clearBuffers->setClearColor(Qt::lightGray);
    new Qt3DRender::QNoDraw(clearBuffers);

    m_guiTag = new Qt3DRender::QLayer; // all gui entities are tagged with this
    m_activeGuiTag = new Qt3DRender::QLayer; // active gui entities - added/removed to entities dynamically by imguimanager
    m_guiTechniqueFilterKey = new Qt3DRender::QFilterKey;
    m_guiTechniqueFilterKey->setName(QLatin1String("gui"));
    m_guiTechniqueFilterKey->setValue(QLatin1String("1"));

    // The normal scene, filter for 'forward' which is set by the "default" materials in Qt3DExtras.
    // Change the key (or switch to RenderPassFilter, extend for multi-pass, etc.) when using custom materials.
    Qt3DRender::QCameraSelector *cameraSel = new Qt3DRender::QCameraSelector(viewport);
    m_sceneCamera = new Qt3DRender::QCamera;
    m_sceneCamera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    m_sceneCamera->setFieldOfView(60);
    m_sceneCamera->setPosition(QVector3D(0, 0, 10));
    m_sceneCamera->setViewCenter(QVector3D(0, 0, 0));
    m_sceneCamera->setNearPlane(10);
    m_sceneCamera->setFarPlane(5000);
    m_sceneCamera->setAspectRatio(width() / float(height()));
    cameraSel->setCamera(m_sceneCamera);
    Qt3DRender::QTechniqueFilter *tfilter = new Qt3DRender::QTechniqueFilter(cameraSel);
    Qt3DRender::QFilterKey *tfilterkey = new Qt3DRender::QFilterKey;
    tfilterkey->setName(QLatin1String("renderingStyle"));
    tfilterkey->setValue(QLatin1String("forward"));
    tfilter->addMatch(tfilterkey);
    // Skip all gui entities in this pass.
    Qt3DRender::QLayerFilter *lfilter = new Qt3DRender::QLayerFilter(tfilter);
    lfilter->setFilterMode(Qt3DRender::QLayerFilter::DiscardAnyMatchingLayers);
    lfilter->addLayer(m_guiTag);

    // The gui pass.
    tfilter = new Qt3DRender::QTechniqueFilter(viewport);
    tfilter->addMatch(m_guiTechniqueFilterKey);
    cameraSel = new Qt3DRender::QCameraSelector(tfilter);
    m_guiCamera = new Qt3DRender::QCamera;
    m_guiCamera->setProjectionType(Qt3DRender::QCameraLens::OrthographicProjection);
    m_guiCamera->setLeft(0);
    m_guiCamera->setRight(width() * devicePixelRatio());
    m_guiCamera->setTop(0);
    m_guiCamera->setBottom(height() * devicePixelRatio());
    m_guiCamera->setNearPlane(-1);
    m_guiCamera->setFarPlane(1);
    cameraSel->setCamera(m_guiCamera);
    // imgui provides "draw calls" in back-to-front order but that will get
    // lost in the pipeline with Qt3D. Ensure via a SortPolicy instead.
    Qt3DRender::QSortPolicy *sortPolicy = new Qt3DRender::QSortPolicy(cameraSel);
    sortPolicy->setSortTypes(QVector<Qt3DRender::QSortPolicy::SortType>() << Qt3DRender::QSortPolicy::BackToFront);
    // Only include entities for the _active_ gui in this pass.
    lfilter = new Qt3DRender::QLayerFilter(sortPolicy);
    lfilter->addLayer(m_activeGuiTag);

    targetSel->setSurface(this);
    frameGraphComponent->setActiveFrameGraph(targetSel);
    m_rootEntity->addComponent(frameGraphComponent);

    Qt3DInput::QInputSettings *inputSettings = new Qt3DInput::QInputSettings;
    inputSettings->setEventSource(this);
    m_rootEntity->addComponent(inputSettings);
}
