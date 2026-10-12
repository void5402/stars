#include "callbacks.hpp"


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {

  SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Window *window;
  SDL_Renderer *renderer;

  if (!SDL_CreateWindowAndRenderer("????", 512, 512,
                                   SDL_WINDOW_FULLSCREEN | SDL_WINDOW_VULKAN,
                                   &window, &renderer)) {
    SDL_Log("Failed to make window or renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderVSync(renderer, 1);



  mystuff *stuff = new mystuff;
  stuff->Wnindow = window;
  stuff->Renderer = renderer;
  *appstate = stuff;

  return SDL_APP_CONTINUE;
}