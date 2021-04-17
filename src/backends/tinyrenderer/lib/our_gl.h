#pragma once

#include <vector>
#include "geometry.h"
#include <SDL.h>
#include "tgaimage.h"
#include "../argb.h"

namespace TinyScene {

    void viewport(const int x, const int y, const int w, const int h);
    void projection(const double coeff = 0); // coeff = -1/c
    void lookat(const vec3 eye, const vec3 center, const vec3 up);

    struct IShader {
        virtual vec4 vertex(const int iface, const int nthvert) = 0;
        virtual bool fragment(const vec3 bar, TGAColor& color) = 0;
    };

    void triangle(const vec4 clip_verts[3], IShader& shader, SDL_Surface* image, int w, int h, std::vector<double>& zbuffer);

}