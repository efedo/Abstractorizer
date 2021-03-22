#include "sdl_utils.h"

//Uint32 get_pixel32(SDL_Surface* surface, int x, int y)
//{
//    //Convert the pixels to 32 bit
//    Uint32* pixels = (Uint32*)surface->pixels;
//
//    //Get the requested pixel
//    return pixels[(y * surface->w) + x];
//}

void set_pixelRGB(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    Uint32 pixel = SDL_MapRGB(surface->format, r, g, b);
    Uint32* const target_pixel = (Uint32*)((Uint8*)surface->pixels
        + y * surface->pitch
        + x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}


void set_pixel32(SDL_Surface* surface, int x, int y, SDL_Color color)
{
    Uint32* pixels = (Uint32*)surface->pixels;
    pixels[(y * surface->w) + x] = SDL_MapRGB(surface->format, color.r, color.g, color.b);
}

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
    return (a << 24) + (b << 16) + (g << 8) + r;
}

void unpack_color(const uint32_t& color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) {
    r = (color >> 0) & 255;
    g = (color >> 8) & 255;
    b = (color >> 16) & 255;
    a = (color >> 24) & 255;
}