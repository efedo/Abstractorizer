// modified from https://github.com/rdhafidh/Urho3DInsideQWidget/blob/master/urho3dwidget.h

#pragma once

#include <C:/Programming/lib/Urho3D/out/build/x64-Debug/include/Urho3D/Container/Ptr.h>
//#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3d/Core/Context.h>
#include <QWidget>

#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Math/StringHash.h>

class Urho3DWidgetInternal : public QWidget, public Urho3D::Object {
    Q_OBJECT

        URHO3D_OBJECT(Urho3DWidgetInternal, Urho3D::Object)
public:
    explicit Urho3DWidgetInternal(Urho3D::Context* context, QWidget* parent = nullptr);
    ~Urho3DWidgetInternal();

    void Start();
    void Stop();
    void Setup();
Q_SIGNALS:

public Q_SLOTS:

protected:
    void closeEvent(QCloseEvent* e);
    void timerEvent(QTimerEvent* e);
    void resizeEvent(QResizeEvent* e);
    void keyPressEvent(QKeyEvent* e);

private:
    void HandleUpdate(Urho3D::StringHash eventType,
        Urho3D::VariantMap& eventData);
    void createScene();
    void setupViewport();
    void MoveCamera();

    Urho3D::SharedPtr<Urho3D::Engine> engine_;
    Urho3D::SharedPtr<Urho3D::Scene> scene_;
    Urho3D::SharedPtr<Urho3D::Node> cameraNode_;
    /// Camera yaw angle.
    float yaw_;
    /// Camera pitch angle.
    float pitch_;
    int timerId;
    float timeStep = 1;
    // Movement speed as world units per second
    const float MOVE_SPEED = 40.0f;
};
