#include "zauonlokrenderer_widget_internal.h"
#include <thread>
#include <iostream>
#include <stdint.h>
#include <iostream>
#include <QWindow>
#include <QVBoxLayout>
#include <QProcess>

extern "C" {
    int blahblahblah();
    int zauonlokmain(int argc, char* argv[]);
}

ZauonlokRendererWidgetInternal::ZauonlokRendererWidgetInternal() {

    blahblahblah();

    uint64_t WindowID;

    zauonlokmain(0, 0);

    //auto ApplicationProcess = new QProcess();
    //ApplicationProcess->start("Viewer", 0);


    ////std::cout << "Zauonlok starting\n";
    //std::thread first(zauonlok_main);
    //first.detach();
    //uint64_t handle = 0;
    //uint64_t iters = 0;
    //while (!handle) {
    //    ++iters;
    //    handle = get_zauonlok_handle();
    //    if (!(iters % 1000)) {
    //        std::cout << "Iters: " << iters / 1000 << "\n";
    //    }    
    //}
    //
    //std::cout << "uint64_t handle: " << handle << "\n";
    //HWND native_window_id = (HWND)handle;
    //std::cout << "HWND handle: " << native_window_id << "\n";

    //QWindow* window = QWindow::fromWinId((WId)(native_window_id));
    //window->setFlags(Qt::FramelessWindowHint);
    //QWidget* widget = QWidget::createWindowContainer(window);
    //QVBoxLayout* layout = new QVBoxLayout(this);
    //this->setLayout(layout);
    //layout->setMargin(0);
    //layout->addWidget(widget);
}

//
//void ZauonlokRendererWidgetInternal::render() {
//    while (!stoprendering) {
//        setupView();
//        std::fill(((uint32_t*)(surface->pixels)), ((uint32_t*)(surface->pixels)) + width * height, 0);
//
//        for (int i = 0; i < model->nfaces(); i++) { // for every triangle
//            TinyScene::vec4 clip_vert[3]; // triangle coordinates (clip coordinates), written by VS, read by FS
//            for (int j = 0; j < 3; j++) {
//                clip_vert[j] = shader->vertex(i, j); // call the vertex shader for each triangle vertex
//            }
//            triangle(clip_vert, *shader, surface, width, height, zbuffer); // actual rasterization routine call
//        }
//
//        for (int j = 0; j < height; ++j) {
//            for (int i = 0; i < width; ++i) {
//                if (i == 50) {
//                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 255, 0, 0);
//                }
//                if (i == 100) {
//                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 0, 255, 0);
//                }
//                if (i == 150) {
//                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 0, 0, 255);
//                }
//                if (j == 50) {
//                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 255, 0, 255);
//                }
//                if (j == 100) {
//                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 255, 255, 0);
//                }
//                if (j == 150) {
//                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 0, 255, 255);
//                }
//            }
//        }
//        std::cout << "Tiny renderer produced frame\n";
//        //if (texture) {
//        //    SDL_DestroyTexture(texture);
//        //    texture = 0;
//        //}
//        //texture = SDL_CreateTextureFromSurface(renderer, surface);
//        texturemutex.lock();
//        SDL_UpdateTexture(texture, NULL, surface->pixels, width * 4);
//        texturemutex.unlock();
//    }
//}
//
//void ZauonlokRendererWidgetInternal::Update() {
//    texturemutex.lock();
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//    SDL_RenderClear(renderer);
//    if (texture) {
//        SDL_RenderCopy(renderer, texture, NULL, NULL);
//    }
//    SDL_RenderPresent(renderer);
//    texturemutex.unlock();
//}
//
//ZauonlokRendererWidgetInternal::~ZauonlokRendererWidgetInternal() {
//    frameTimer.stop();
//    stoprendering = true;
//    renderthread.join();
//
//    if (model) delete model;
//    if (shader) delete shader;
//
//    texturemutex.lock();
//
//    if (texture) {
//        SDL_DestroyTexture(texture);
//        texture = 0;
//    }
//
//    texturemutex.unlock();
//
//    if (surface) {
//        SDL_FreeSurface(surface);
//        surface = 0;
//    }
//}