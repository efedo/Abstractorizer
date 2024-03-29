// Original code from https://github.com/alpqr/imgui-qt3d

#include "qt3d_window.h"
#include "qt3d_imgui_manager.h"
#include "gui.h"
#include "imgui/imgui.h"

#include <Qt3DCore/QEntity>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QImage>
#include <QTransform>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QAbstractTextureImage>
#include <Qt3DRender/QTextureImageDataGenerator>
#include <QBuffer>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QTechnique>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QFilterKey>
#include <Qt3DRender/QTechniqueFilter>
#include <Qt3DRender/QGraphicsApiFilter>
#include <Qt3DRender/QLayer>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QDepthTest>
#include <Qt3DRender/QBlendEquation>
#include <Qt3DRender/QBlendEquationArguments>
#include <Qt3DRender/QNoDepthMask>
#include <Qt3DRender/QCullFace>
#include <Qt3DRender/QColorMask>
#include <Qt3DRender/QScissorTest>
#include <Qt3DLogic/QFrameAction>
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
#include <Qt3DRender/QNoDraw>
#include <Qt3DRender/QLayerFilter>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QSortPolicy>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QPropertyAnimation>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QBuffer>

class ImguiTextureImageDataGen : public Qt3DRender::QTextureImageDataGenerator
{
public:
    ImguiTextureImageDataGen(const QImage &image) : m_image(image) { }

    Qt3DRender::QTextureImageDataPtr operator()() override
    {
        Qt3DRender::QTextureImageDataPtr textureData = Qt3DRender::QTextureImageDataPtr::create();
        textureData->setImage(m_image);
        return textureData;
    }
    bool operator==(const Qt3DRender::QTextureImageDataGenerator &other) const override
    {
        const ImguiTextureImageDataGen *otherFunctor = functor_cast<ImguiTextureImageDataGen>(&other);
        return otherFunctor && otherFunctor->m_image == m_image;
    }

    QT3D_FUNCTOR(ImguiTextureImageDataGen)

private:
    QImage m_image;
};

class TextureImage : public Qt3DRender::QAbstractTextureImage
{
public:
    TextureImage(const QImage &image) { m_gen = QSharedPointer<ImguiTextureImageDataGen>::create(image); }

private:
    Qt3DRender::QTextureImageDataGeneratorPtr dataGenerator() const override { return m_gen; }

    Qt3DRender::QTextureImageDataGeneratorPtr m_gen;
};

Qt3DImguiManager::Qt3DImguiManager(Qt3DWindow & parentWindow, Qt3DCore::QEntity & rootEntity)
    : _parentWindow(parentWindow), _rootEntity(rootEntity)
{
    _gui = new Gui(*this);

    Qt3DLogic::QFrameAction* frameUpdater = new Qt3DLogic::QFrameAction;
    QObject::connect(frameUpdater, &Qt3DLogic::QFrameAction::triggered, this, &Qt3DImguiManager::updateFrame);
    _rootEntity.addComponent(frameUpdater);

    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int w, h;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);

    m_atlasTex = new Qt3DRender::QTexture2D;
    m_atlasTex->setFormat(Qt3DRender::QAbstractTexture::RGBA8_UNorm);
    m_atlasTex->setWidth(w);
    m_atlasTex->setHeight(h);
    m_atlasTex->setMinificationFilter(Qt3DRender::QAbstractTexture::Linear);
    m_atlasTex->setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);

    QImage wrapperImg((const uchar*)pixels, w, h, QImage::Format_RGBA8888);
    m_atlasTex->addTextureImage(new TextureImage(wrapperImg));

    io.Fonts->SetTexID(m_atlasTex);

    setupInputEventSource();
}

Qt3DImguiManager::OutputInfo Qt3DImguiManager::getOutputInfo() {
    OutputInfo outputInfo;
    outputInfo.size = _parentWindow.size();
    outputInfo.dpr = _parentWindow.devicePixelRatio();
    outputInfo.guiTag = _parentWindow.guiTag();
    outputInfo.activeGuiTag = _parentWindow.activeGuiTag();
    outputInfo.guiTechniqueFilterKey = _parentWindow.guiTechniqueFilterKey();
    return outputInfo;
};

void Qt3DImguiManager::pickTexSlot(Qt3DRender::QPickEvent* /*pickEvent*/) {
    this->setEnabled(!isEnabled());
    toggleTextMat->setDiffuse(isEnabled() ? Qt::green : Qt::red);
}

void Qt3DImguiManager::createScene() {

    // Maybe should use _parent here instead of &_rootEntity
    Qt3DCore::QEntity* cube = new Qt3DCore::QEntity(&_rootEntity);
    Qt3DExtras::QCuboidMesh* cubeGeom = new Qt3DExtras::QCuboidMesh;
    cubeGeom->setXExtent(2);
    cubeGeom->setYExtent(2);
    cubeGeom->setZExtent(2);
    Qt3DCore::QTransform* cubeTrans = new Qt3DCore::QTransform;
    cubeTrans->setTranslation(QVector3D(18, 5, -10));
    Qt3DExtras::QPhongMaterial* cubeMat = new Qt3DExtras::QPhongMaterial;
    cube->addComponent(cubeGeom);
    cube->addComponent(cubeTrans);
    cube->addComponent(cubeMat);

    Qt3DCore::QEntity* cube2 = new Qt3DCore::QEntity(&_rootEntity);
    // or, why have a cube when we can have a torus instead
    Qt3DExtras::QTorusMesh* cubeGeom2 = new Qt3DExtras::QTorusMesh;
    cubeGeom2->setRadius(5);
    cubeGeom2->setMinorRadius(1);
    cubeGeom2->setRings(100);
    cubeGeom2->setSlices(20);
    Qt3DCore::QTransform* cubeTrans2 = new Qt3DCore::QTransform;
    cubeTrans2->setTranslation(QVector3D(-15, -4, -20));
    cube2->addComponent(cubeGeom2);
    cube2->addComponent(cubeTrans2);
    cube2->addComponent(cubeMat);

    auto rotAnim = [](QObject* obj, const QByteArray& name, float start, float end, int duration) {
        QPropertyAnimation* anim = new QPropertyAnimation(obj, name);
        anim->setStartValue(start);
        anim->setEndValue(end);
        anim->setDuration(duration);
        anim->setLoopCount(-1);
        anim->start();
    };
    rotAnim(cubeTrans, "rotationX", 0.0f, 360.0f, 5000);
    rotAnim(cubeTrans2, "rotationY", 0.0f, 360.0f, 5000);

    // an entity that toggles the gui when pressed.
    // replace with a QText2DEntity some day when it actually works. in the meantime a sphere will do.
    Qt3DCore::QEntity* toggleText = new Qt3DCore::QEntity(&_rootEntity);
    Qt3DExtras::QSphereMesh* toggleTextGeom = new Qt3DExtras::QSphereMesh;
    Qt3DCore::QTransform* toggleTextTrans = new Qt3DCore::QTransform;
    toggleTextTrans->setTranslation(QVector3D(-14, 7, -5));
    toggleTextTrans->setScale(0.5f);
    toggleTextMat = new Qt3DExtras::QPhongMaterial;
    toggleTextMat->setDiffuse(this->isEnabled() ? Qt::green : Qt::red);
    toggleText->addComponent(toggleTextGeom);
    toggleText->addComponent(toggleTextTrans);
    toggleText->addComponent(toggleTextMat);
    Qt3DRender::QObjectPicker* toggleTextPicker = new Qt3DRender::QObjectPicker;
    QObject::connect(toggleTextPicker, &Qt3DRender::QObjectPicker::pressed, this, &Qt3DImguiManager::pickTexSlot);
    //[toggleTextMat, &_guiMgr](Qt3DRender::QPickEvent*) {
    //_guiMgr.setEnabled(!_guiMgr.isEnabled());
    //toggleTextMat->setDiffuse(_guiMgr.isEnabled() ? Qt::green : Qt::red);
    //});
    toggleText->addComponent(toggleTextPicker);
}

void Qt3DImguiManager::updateFrame() {
    if (!m_enabled) return;

    m_outputInfo = getOutputInfo();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = m_outputInfo.size.width() * m_outputInfo.dpr;
    io.DisplaySize.y = m_outputInfo.size.height() * m_outputInfo.dpr;

    updateInput();

    // One weakness regarding input is that io.WantCaptureMouse/Keyboard
    // cannot really be supported. We could check for them after the
    // NewFrame() call below, but there is nothing we can do at this stage,
    // the Qt events have already been dispatched and processed.

    ImGui::NewFrame();
    //_guiMgr.setFrameFunc(std::bind(&Gui::frame, &gui, std::placeholders::_1));
    _gui->frame(&_rootEntity);
    ImGui::Render();

    update3D();
}

// To be called when the Qt 3D scene goes down (and thus destroys the objects
// Qt3DImguiManager references) but the Qt3DImguiManager instance will be reused later
// on. Must be followed by a call to initialize().
void Qt3DImguiManager::releaseResources()
{
    // assume that everything starting from our root entity is already gone
    rpd = SharedRenderPassData();
    m_cmdList.clear();
}

void Qt3DImguiManager::resizePool(CmdListEntry *e, int newSize)
{
    Q_ASSERT(m_outputInfo.guiTag && m_outputInfo.activeGuiTag);

    const int oldSize = e->cmds.count();
    if (newSize > oldSize) {
        e->cmds.resize(newSize);
        for (int i = oldSize; i < newSize; ++i) {
            CmdEntry *ecmd = &e->cmds[i];
            Qt3DCore::QEntity *entity = new Qt3DCore::QEntity(&_rootEntity);
            entity->addComponent(m_outputInfo.guiTag);
            Qt3DRender::QMaterial *material = buildMaterial(&ecmd->scissor);
            entity->addComponent(material);
            Qt3DRender::QGeometryRenderer *geomRenderer = new Qt3DRender::QGeometryRenderer;
            entity->addComponent(geomRenderer);
            Qt3DCore::QTransform *transform = new Qt3DCore::QTransform;
            entity->addComponent(transform);
            Qt3DRender::QParameter *texParam = new Qt3DRender::QParameter;
            texParam->setName(QLatin1String("tex"));
            texParam->setValue(QVariant::fromValue(m_atlasTex)); // needs a valid initial value
            material->addParameter(texParam);
            ecmd->entity = entity;
            ecmd->material = material;
            ecmd->geomRenderer = geomRenderer;
            ecmd->transform = transform;
            ecmd->texParam = texParam;
        }
    }

    // make sure only entities from the first newSize entries are tagged as active gui
    if (e->activeSize > newSize) {
        for (int i = newSize; i < e->activeSize; ++i) {
            e->cmds[i].entity->removeComponent(m_outputInfo.activeGuiTag);
            updateGeometry(e, i, 0, 0, 0, nullptr);
        }
    } else if (e->activeSize < newSize) {
        for (int i = e->activeSize; i < newSize; ++i)
            e->cmds[i].entity->addComponent(m_outputInfo.activeGuiTag);
        // up to the caller to do updateGeometry for [0..newSize-1]
    }

    e->activeSize = newSize;
}

void Qt3DImguiManager::updateGeometry(CmdListEntry *e, int idx, uint elemCount, int vertexCount, int indexCount, const void *indexOffset)
{
    Qt3DRender::QGeometryRenderer *gr = e->cmds[idx].geomRenderer;
    Qt3DRender::QGeometry *g = gr->geometry();

    if (!g) {
        gr->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
        g = new Qt3DRender::QGeometry;

        for (int i = 0; i < 4; ++i)
            g->addAttribute(new Qt3DRender::QAttribute);

        const int vsize = 3; // assumes ImDrawVert was overridden in imconfig.h
        Q_ASSERT(sizeof(ImDrawVert) == (vsize + 2) * sizeof(float) + sizeof(ImU32));

        const QVector<Qt3DRender::QAttribute *> attrs = g->attributes();
        Qt3DRender::QAttribute *attr = attrs[0];
        attr->setBuffer(e->vbuf);
        attr->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
        attr->setVertexBaseType(Qt3DRender::QAttribute::Float);
        attr->setVertexSize(vsize);
        attr->setCount(vertexCount);
        attr->setByteOffset(0);
        attr->setByteStride(sizeof(ImDrawVert));

        attr = attrs[1];
        attr->setBuffer(e->vbuf);
        attr->setName(Qt3DRender::QAttribute::defaultTextureCoordinateAttributeName());
        attr->setVertexBaseType(Qt3DRender::QAttribute::Float);
        attr->setVertexSize(2);
        attr->setCount(vertexCount);
        attr->setByteOffset(vsize * sizeof(float));
        attr->setByteStride(sizeof(ImDrawVert));

        attr = attrs[2];
        attr->setBuffer(e->vbuf);
        attr->setName(Qt3DRender::QAttribute::defaultColorAttributeName());
        attr->setVertexBaseType(Qt3DRender::QAttribute::UnsignedByte);
        attr->setVertexSize(4);
        attr->setCount(vertexCount);
        attr->setByteOffset((vsize + 2) * sizeof(float));
        attr->setByteStride(sizeof(ImDrawVert));

        attr = attrs[3];
        attr->setBuffer(e->ibuf);
        attr->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
        attr->setVertexBaseType(sizeof(ImDrawIdx) == 2 ? Qt3DRender::QAttribute::UnsignedShort : Qt3DRender::QAttribute::UnsignedInt);
        attr->setVertexSize(1);
        attr->setCount(indexCount);
        attr->setByteOffset((quintptr) indexOffset);
        attr->setByteStride(0);

        gr->setGeometry(g);
    } else {
        // only update the potentially changing properties afterwards
        const QVector<Qt3DRender::QAttribute *> attrs = g->attributes();
        Q_ASSERT(attrs.count() == 4);

        Qt3DRender::QAttribute *attr = attrs[0];
        attr->setBuffer(e->vbuf);
        attr->setCount(vertexCount);

        attr = attrs[1];
        attr->setBuffer(e->vbuf);
        attr->setCount(vertexCount);

        attr = attrs[2];
        attr->setBuffer(e->vbuf);
        attr->setCount(vertexCount);

        attr = attrs[3];
        attr->setBuffer(e->ibuf);
        attr->setCount(indexCount);
        attr->setByteOffset((quintptr) indexOffset);
    }

    gr->setVertexCount(elemCount);
}

// called once per frame, performs gui-related updates for the scene
void Qt3DImguiManager::update3D()
{
    ImDrawData *d = ImGui::GetDrawData();
    ImGuiIO &io = ImGui::GetIO();

    if (m_cmdList.count() < d->CmdListsCount) {
        m_cmdList.resize(d->CmdListsCount);
    } else {
        // Make sure all unused entities are untagged.
        for (int n = d->CmdListsCount; n < m_cmdList.count(); ++n)
            resizePool(&m_cmdList[n], 0);
    }

    d->ScaleClipRects(ImVec2(m_scale, m_scale));

    // CmdLists is in back-to-front order, assign z values accordingly
    const float zstep = 0.01f;
    float z = -1000;

    for (int n = 0; n < d->CmdListsCount; ++n) {
        const ImDrawList *cmdList = d->CmdLists[n];
        CmdListEntry *e = &m_cmdList[n];

        if (!e->vbuf) {
            e->vbuf = new Qt3DRender::QBuffer;
            e->vbuf->setUsage(Qt3DRender::QBuffer::StreamDraw);
        }
        // NB! must make a copy in any case, fromRawData would be wrong here
        // even if we did not need to insert the dummy z values.
        QByteArray vdata((const char *) cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
        // Initialize Z values. The shader does not need it but some Qt3D stuff (back-to-front sorting, bounding volumes) does.
        {
            ImDrawVert *v = (ImDrawVert *) vdata.data();
            uint idxOffset = 0;
            // Assign a Z value per draw call, not per draw call list.
            // This assumes no vertices are shared between the draw calls.
            for (int i = 0; i < cmdList->CmdBuffer.Size; ++i) {
                const ImDrawCmd *cmd = &cmdList->CmdBuffer[i];
                if (!cmd->UserCallback) {
                    for (uint ei = 0; ei < cmd->ElemCount; ++ei) {
                        ImDrawIdx idx = cmdList->IdxBuffer.Data[idxOffset + ei];
                        v[idx].z = z;
                    }
                }
                idxOffset += cmd->ElemCount;
                z += zstep;
            }
        }
        e->vbuf->setData(vdata);

        if (!e->ibuf) {
            e->ibuf = new Qt3DRender::QBuffer;
            e->ibuf->setUsage(Qt3DRender::QBuffer::StreamDraw);
        }
        // same here
        e->ibuf->setData(QByteArray((const char *) cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx)));

        // Ensure the needed number of entities and components are available; tag/untag as necessary
        resizePool(e, cmdList->CmdBuffer.Size);

        const ImDrawIdx *indexBufOffset = nullptr;
        for (int i = 0; i < cmdList->CmdBuffer.Size; ++i) {
            const ImDrawCmd *cmd = &cmdList->CmdBuffer[i];
            CmdEntry *ecmd = &e->cmds[i];
            if (!cmd->UserCallback) {
                updateGeometry(e, i, cmd->ElemCount, cmdList->VtxBuffer.Size, cmdList->IdxBuffer.Size, indexBufOffset);
                ecmd->texParam->setValue(QVariant::fromValue(static_cast<Qt3DRender::QAbstractTexture *>(cmd->TextureId)));

                Qt3DRender::QScissorTest *scissor = ecmd->scissor;
                scissor->setLeft(cmd->ClipRect.x);
                scissor->setBottom(io.DisplaySize.y - cmd->ClipRect.w);
                scissor->setWidth(cmd->ClipRect.z - cmd->ClipRect.x);
                scissor->setHeight(cmd->ClipRect.w - cmd->ClipRect.y);

                Qt3DCore::QTransform *transform = ecmd->transform;
                transform->setScale(m_scale);
            } else {
                cmd->UserCallback(cmdList, cmd);
            }
            indexBufOffset += cmd->ElemCount;
        }
    }
}

static const char *vertSrcES2 =
        "attribute vec4 vertexPosition;\n"
        "attribute vec4 vertexColor;\n"
        "attribute vec2 vertexTexCoord;\n"
        "varying vec2 uv;\n"
        "varying vec4 color;\n"
        "uniform mat4 projectionMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "void main() {\n"
        "    uv = vertexTexCoord;\n"
        "    color = vertexColor;\n"
        "    gl_Position = projectionMatrix * modelMatrix * vec4(vertexPosition.xy, 0.0, 1.0);\n"
        "}\n";

static const char *fragSrcES2 =
        "uniform sampler2D tex;\n"
        "varying highp vec2 uv;\n"
        "varying highp vec4 color;\n"
        "void main() {\n"
        "    gl_FragColor = color * texture2D(tex, uv);\n"
        "}\n";

static const char *vertSrcGL3 =
        "#version 150\n"
        "in vec4 vertexPosition;\n"
        "in vec4 vertexColor;\n"
        "in vec2 vertexTexCoord;\n"
        "out vec2 uv;\n"
        "out vec4 color;\n"
        "uniform mat4 projectionMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "void main() {\n"
        "    uv = vertexTexCoord;\n"
        "    color = vertexColor;\n"
        "    gl_Position = projectionMatrix * modelMatrix * vec4(vertexPosition.xy, 0.0, 1.0);\n"
        "}\n";

static const char *fragSrcGL3 =
        "#version 150\n"
        "uniform sampler2D tex;\n"
        "in vec2 uv;\n"
        "in vec4 color;\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "    fragColor = color * texture(tex, uv);\n"
        "}\n";

Qt3DRender::QMaterial *Qt3DImguiManager::buildMaterial(Qt3DRender::QScissorTest **scissor)
{
    Qt3DRender::QMaterial *material = new Qt3DRender::QMaterial;
    Qt3DRender::QEffect *effect = new Qt3DRender::QEffect;

    auto buildShaderProgram = [](const char *vs, const char *fs) {
        Qt3DRender::QShaderProgram *prog = new Qt3DRender::QShaderProgram;
        prog->setVertexShaderCode(vs);
        prog->setFragmentShaderCode(fs);
        return prog;
    };

    if (!rpd.valid) {
        rpd.valid = true;

        rpd.progES2 = buildShaderProgram(vertSrcES2, fragSrcES2);
        rpd.progGL3 = buildShaderProgram(vertSrcGL3, fragSrcGL3);

        // the framegraph is expected to filter for this key in its gui pass
        rpd.techniqueFilterKey = m_outputInfo.guiTechniqueFilterKey;

        rpd.depthTest = new Qt3DRender::QDepthTest;
        rpd.depthTest->setDepthFunction(Qt3DRender::QDepthTest::Always);

        rpd.noDepthWrite = new Qt3DRender::QNoDepthMask;

        rpd.blendFunc = new Qt3DRender::QBlendEquation;
        rpd.blendArgs = new Qt3DRender::QBlendEquationArguments;
        rpd.blendFunc->setBlendFunction(Qt3DRender::QBlendEquation::Add);
        rpd.blendArgs->setSourceRgb(Qt3DRender::QBlendEquationArguments::SourceAlpha);
        rpd.blendArgs->setDestinationRgb(Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);
        rpd.blendArgs->setSourceAlpha(Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);
        rpd.blendArgs->setDestinationAlpha(Qt3DRender::QBlendEquationArguments::Zero);

        rpd.cullFace = new Qt3DRender::QCullFace;
        rpd.cullFace->setMode(Qt3DRender::QCullFace::NoCulling);

        rpd.colorMask = new Qt3DRender::QColorMask;
        rpd.colorMask->setAlphaMasked(false);
    }

    *scissor = new Qt3DRender::QScissorTest;

    // have two techniques: one for OpenGL ES (2.0+) and one for OpenGL core (3.2+)

    auto buildRenderPass = [this, scissor](Qt3DRender::QShaderProgram *prog) {
        Qt3DRender::QRenderPass *rpass = new Qt3DRender::QRenderPass;
        rpass->setShaderProgram(prog);

        rpass->addRenderState(rpd.depthTest);
        rpass->addRenderState(rpd.noDepthWrite);
        rpass->addRenderState(rpd.blendFunc);
        rpass->addRenderState(rpd.blendArgs);
        rpass->addRenderState(rpd.cullFace);
        rpass->addRenderState(rpd.colorMask);
        rpass->addRenderState(*scissor);

        // Our setEnabled() maps to QNode::setEnabled() on the QRenderPass
        // hence the need for keeping track. This is simpler than playing with
        // QLayer on entities.
        rpd.enabledToggle.append(rpass);

        return rpass;
    };

    Qt3DRender::QTechnique *techniqueES2 = new Qt3DRender::QTechnique;
    techniqueES2->addFilterKey(rpd.techniqueFilterKey);
    Qt3DRender::QGraphicsApiFilter *apiFilterES2 = techniqueES2->graphicsApiFilter();
    apiFilterES2->setApi(Qt3DRender::QGraphicsApiFilter::OpenGLES);
    apiFilterES2->setMajorVersion(2);
    apiFilterES2->setMinorVersion(0);
    apiFilterES2->setProfile(Qt3DRender::QGraphicsApiFilter::NoProfile);
    techniqueES2->addRenderPass(buildRenderPass(rpd.progES2));

    Qt3DRender::QTechnique *techniqueGL3 = new Qt3DRender::QTechnique;
    techniqueGL3->addFilterKey(rpd.techniqueFilterKey);
    Qt3DRender::QGraphicsApiFilter *apiFilterGL3 = techniqueGL3->graphicsApiFilter();
    apiFilterGL3->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    apiFilterGL3->setMajorVersion(3);
    apiFilterGL3->setMinorVersion(2);
    apiFilterGL3->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);
    techniqueGL3->addRenderPass(buildRenderPass(rpd.progGL3));

    effect->addTechnique(techniqueES2);
    effect->addTechnique(techniqueGL3);

    material->setEffect(effect);

    return material;
}

#define FIRSTSPECKEY (0x01000000)
#define LASTSPECKEY (0x01000017)
#define MAPSPECKEY(k) ((k) - FIRSTSPECKEY + 256)

// Do not bother with 3D picking, assume the UI is displayed 1:1 in the window.
class ImguiInputEventFilter : public QObject
{
public:
    ImguiInputEventFilter()
    {
        memset(keyDown, 0, sizeof(keyDown));
    }

    bool eventFilter(QObject *watched, QEvent *event) override;

    QPointF mousePos;
    Qt::MouseButtons mouseButtonsDown = Qt::NoButton;
    float mouseWheel = 0;
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;
    bool keyDown[256 + (LASTSPECKEY - FIRSTSPECKEY + 1)];
    QString keyText;
    bool enabled = true;
};

bool ImguiInputEventFilter::eventFilter(QObject *, QEvent *event)
{
    if (!enabled)
        return false;

    switch (event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseMove:
    case QEvent::MouseButtonRelease:
    {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        mousePos = me->pos();
        mouseButtonsDown = me->buttons();
        modifiers = me->modifiers();
    }
        break;

    case QEvent::Wheel:
    {
        QWheelEvent *we = static_cast<QWheelEvent *>(event);
        mouseWheel += we->angleDelta().y() / 120.0f;
    }
        break;

    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    {
        const bool down = event->type() == QEvent::KeyPress;
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        modifiers = ke->modifiers();
        if (down)
            keyText.append(ke->text());
        int k = ke->key();
        if (k <= 0xFF)
            keyDown[k] = down;
        else if (k >= FIRSTSPECKEY && k <= LASTSPECKEY)
            keyDown[MAPSPECKEY(k)] = down;
    }
        break;

    default:
        break;
    }

    return false;
}

Qt3DImguiManager::~Qt3DImguiManager()
{
    delete m_inputEventFilter;
    if (_gui) delete _gui;
}

void Qt3DImguiManager::setupInputEventSource()
{
    if (m_inputEventFilter) _parentWindow.removeEventFilter(m_inputEventFilter);
    if (!m_inputEventFilter) m_inputEventFilter = new ImguiInputEventFilter;
    _parentWindow.installEventFilter(m_inputEventFilter);
}

void Qt3DImguiManager::updateInput()
{
    if (!m_inputEventFilter)
        return;

    ImGuiIO &io = ImGui::GetIO();

    if (!m_inputInitialized) {
        m_inputInitialized = true;

        io.KeyMap[ImGuiKey_Tab] = MAPSPECKEY(Qt::Key_Tab);
        io.KeyMap[ImGuiKey_LeftArrow] = MAPSPECKEY(Qt::Key_Left);
        io.KeyMap[ImGuiKey_RightArrow] = MAPSPECKEY(Qt::Key_Right);
        io.KeyMap[ImGuiKey_UpArrow] = MAPSPECKEY(Qt::Key_Up);
        io.KeyMap[ImGuiKey_DownArrow] = MAPSPECKEY(Qt::Key_Down);
        io.KeyMap[ImGuiKey_PageUp] = MAPSPECKEY(Qt::Key_PageUp);
        io.KeyMap[ImGuiKey_PageDown] = MAPSPECKEY(Qt::Key_PageDown);
        io.KeyMap[ImGuiKey_Home] = MAPSPECKEY(Qt::Key_Home);
        io.KeyMap[ImGuiKey_End] = MAPSPECKEY(Qt::Key_End);
        io.KeyMap[ImGuiKey_Delete] = MAPSPECKEY(Qt::Key_Delete);
        io.KeyMap[ImGuiKey_Backspace] = MAPSPECKEY(Qt::Key_Backspace);
        io.KeyMap[ImGuiKey_Enter] = MAPSPECKEY(Qt::Key_Return);
        io.KeyMap[ImGuiKey_Escape] = MAPSPECKEY(Qt::Key_Escape);

        io.KeyMap[ImGuiKey_A] = Qt::Key_A;
        io.KeyMap[ImGuiKey_C] = Qt::Key_C;
        io.KeyMap[ImGuiKey_V] = Qt::Key_V;
        io.KeyMap[ImGuiKey_X] = Qt::Key_X;
        io.KeyMap[ImGuiKey_Y] = Qt::Key_Y;
        io.KeyMap[ImGuiKey_Z] = Qt::Key_Z;
    }

    ImguiInputEventFilter *w = m_inputEventFilter;

    io.MousePos = ImVec2((w->mousePos.x() / m_scale) * m_outputInfo.dpr, (w->mousePos.y() / m_scale) * m_outputInfo.dpr);

    io.MouseDown[0] = w->mouseButtonsDown.testFlag(Qt::LeftButton);
    io.MouseDown[1] = w->mouseButtonsDown.testFlag(Qt::RightButton);
    io.MouseDown[2] = w->mouseButtonsDown.testFlag(Qt::MiddleButton);

    io.MouseWheel = w->mouseWheel;
    w->mouseWheel = 0;

    io.KeyCtrl = w->modifiers.testFlag(Qt::ControlModifier);
    io.KeyShift = w->modifiers.testFlag(Qt::ShiftModifier);
    io.KeyAlt = w->modifiers.testFlag(Qt::AltModifier);
    io.KeySuper = w->modifiers.testFlag(Qt::MetaModifier);

    memcpy(io.KeysDown, w->keyDown, sizeof(w->keyDown));

    if (!w->keyText.isEmpty()) {
        for (const QChar &c : w->keyText) {
            ImWchar u = c.unicode();
            if (u)
                io.AddInputCharacter(u);
        }
        w->keyText.clear();
    }
}

void Qt3DImguiManager::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;

    for (Qt3DCore::QNode *n : rpd.enabledToggle)
        n->setEnabled(m_enabled);

    if (m_inputEventFilter)
        m_inputEventFilter->enabled = m_enabled;
}

void Qt3DImguiManager::setScale(float scale)
{
    m_scale = scale;
}
