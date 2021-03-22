
#include <QResizeEvent>
#include "wicked_widget_internal.h"

WickedWidgetInternal::WickedWidgetInternal()
	: mainComponent(new MainComponent) //tests(new Tests)
{
}

WickedWidgetInternal::~WickedWidgetInternal() {
    if (mainComponent) { //tests
        delete mainComponent;
        mainComponent = 0;
    }
}

void WickedWidgetInternal::exposeEvent(bool visible)
{
}

void WickedWidgetInternal::resizeEvent(QResizeEvent* event)
{
}

void WickedWidgetInternal::runThread() {
    while (!quit)
    {
        mainComponent->Run();
        //std::cout << "Wicked run!\n";
    }
}

WickedWidgetInternalWindows::WickedWidgetInternalWindows() {
    mainComponent->SetWindow(HWND(this->winId()));
    bool quit = false;
    //tests->Run();

    wiRenderer::SetTemporalAAEnabled(true);
    wiScene::LoadModel("../models/teapot.wiscene");

    //wiScene::GetScene().meshes.Create(meshComponent);

    std::thread first(&WickedWidgetInternal::runThread, this);
    first.detach();
}
//
//WickedWidgetInternalSDL::WickedWidgetInternalSDL(QWidget* parent) : SDLWidget(parent), WickedWidgetInternal(parent) {
//    //launchWinMain(this->window);
//}