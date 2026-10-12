#include "callbacks.hpp"

SDL_AppResult SDL_AppIterate(void *appstate) {
  mystuff *stuff = static_cast<mystuff *>(appstate);


  //SDL_SetRenderDrawColor(stuff->Renderer, 0, 0, 0, 255);
  //DL_RenderClear(stuff->Renderer);

  SDL_RenderPresent(stuff->Renderer);
  return SDL_APP_CONTINUE;
}
