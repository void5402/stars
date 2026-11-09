#pragma once
#include <SDL3/SDL.h>
#include <array>
#include <cmath>
#ifdef __clang__
#define constmacro const
#else
#define constmacro constexpr
#endif

inline constmacro int n = 1000;
inline constmacro size_t vertlen = n * 4;
inline constmacro size_t indilen = n * 6;
inline constmacro float maxz = 20000.f;
inline constmacro float FOV_RAD = 90.f * static_cast<float>(M_PI) / 180.f;
inline constmacro float M = 1.f / std::tan(FOV_RAD * 0.5f);
inline constmacro float maxradius = 50.f;
inline constmacro float minradius = 0.f;
inline constmacro float R = (maxradius - minradius) / (M - maxz);
inline constmacro float T = maxradius - R * M;
inline constmacro float Z_WRAP = 25.f;

struct Cluster {
  std::array<float, n> x, y, z; // alignas(16)
};

struct mystuff {
  SDL_Window *Window = nullptr;
  SDL_Renderer *Renderer = nullptr;
  Cluster *cluster = nullptr;
  SDL_Texture *texture = nullptr;
  int w = 0;
  int h = 0;
};

extern "C" {
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]);
SDL_AppResult SDL_AppIterate(void *appstate);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
void SDL_AppQuit(void *appstate, SDL_AppResult result);
}
