#pragma once
#include <SDL3/SDL.h>
#include <array>
#define number_of_stars 3

struct cluster
{
  std::array<float,number_of_stars> x, y, z;
};

struct mystuff
{
  SDL_Window *Wnindow = nullptr;
  SDL_Renderer *Renderer = nullptr;
  int n = number_of_stars;
  cluster *cluster = nullptr;
};

extern "C"
{
  SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]);
  SDL_AppResult SDL_AppIterate(void *appstate);
  SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
  void SDL_AppQuit(void *appstate, SDL_AppResult result);
}