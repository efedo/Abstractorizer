#include "tinyrenderer_widget_internal.h"
#include "lib/main.h"
#include <thread>

void TinyRendererWidgetInternal::setupView() {
    SDL_Surface* wsurface = SDL_GetWindowSurface(sdlwindow);
    if ((width == wsurface->w) && (height == wsurface->h)) {
        return;
    }

    width = wsurface->w;
    height = wsurface->h;

    std::cout << "Resizing texture to width: " << width << " height: " << height << "\n";

    texturemutex.lock();

    if (texture) {
        SDL_DestroyTexture(texture);
        texture = 0;
    }

    texture = SDL_CreateTexture
    (
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );

    texturemutex.unlock();

    if (surface) {
        SDL_FreeSurface(surface);
        surface = 0;
    }
    
    surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_ARGB8888);

    zbuffer.resize(width * height, -std::numeric_limits<double>::max()); // note that the z-buffer is initialized with minimal possible values
    lookat(eye, center, up);                            // build the ModelView matrix
    viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4); // build the Viewport matrix
    projection(-1.f / (eye - center).norm());               // build the Projection matrix
}

TinyRendererWidgetInternal::TinyRendererWidgetInternal() {
    start = SDL_GetPerformanceCounter();
    std::string filename = "resources/obj/boggie/head.obj";
    setupView(); // currently needs to be run once
    model = new Model(filename);
    shader = new Shader(*model);
    renderthread = std::thread(&TinyRendererWidgetInternal::render, this);
}

void TinyRendererWidgetInternal::render() {
    while (!stoprendering) {
        setupView();
        std::fill(((uint32_t*)(surface->pixels)), ((uint32_t*)(surface->pixels)) + width * height, 0);

        for (int i = 0; i < model->nfaces(); i++) { // for every triangle
            vec4 clip_vert[3]; // triangle coordinates (clip coordinates), written by VS, read by FS
            for (int j = 0; j < 3; j++) {
                clip_vert[j] = shader->vertex(i, j); // call the vertex shader for each triangle vertex
            }
            triangle(clip_vert, *shader, surface, width, height, zbuffer); // actual rasterization routine call
        }

        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                if (i == 50) {
                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 255, 0, 0);
                }
                if (i == 100) {
                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 0, 255, 0);
                }
                if (i == 150) {
                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 0, 0, 255);
                }
                if (j == 50) {
                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 255, 0, 255);
                }
                if (j == 100) {
                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 255, 255, 0);
                }
                if (j == 150) {
                    ((uint32_t*)(surface->pixels))[j * width + i] = SDL_MapRGB(surface->format, 0, 255, 255);
                }
            }
        }
        std::cout << "Tiny renderer produced frame\n";
        //if (texture) {
        //    SDL_DestroyTexture(texture);
        //    texture = 0;
        //}
        //texture = SDL_CreateTextureFromSurface(renderer, surface);
        texturemutex.lock();
        SDL_UpdateTexture(texture, NULL, surface->pixels, width * 4);
        texturemutex.unlock();
    }
}

void TinyRendererWidgetInternal::Update() {
    texturemutex.lock();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, NULL);
    }
    SDL_RenderPresent(renderer);
    texturemutex.unlock();
}

TinyRendererWidgetInternal::~TinyRendererWidgetInternal() {
    frameTimer.stop();
    stoprendering = true;
    renderthread.join();

    if (model) delete model;
    if (shader) delete shader;

    texturemutex.lock();

    if (texture) {
        SDL_DestroyTexture(texture);
        texture = 0;
    }

    texturemutex.unlock();

    if (surface) {
        SDL_FreeSurface(surface);
        surface = 0;
    }
}