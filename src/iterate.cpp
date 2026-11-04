#include "callbacks.hpp"

SDL_AppResult SDL_AppIterate(void *appstate)
{
  mystuff *stuff = static_cast<mystuff *>(appstate);
  Cluster &c = *(stuff->cluster);

  





  SDL_SetRenderDrawColor(stuff->Renderer, 0, 0, 0, 255);
  SDL_RenderClear(stuff->Renderer);
  SDL_Vertex vertices[3]={
    {{0  ,  0},{1,0,0,1},{0,0}},
    {{512,  0},{0,0,1,1},{0,0}},
    {{512,512},{0,1,0,1},{0,0}}
  };
  SDL_RenderGeometry(stuff->Renderer, nullptr, vertices, 3, nullptr, 0);

  SDL_RenderPresent(stuff->Renderer);
  return SDL_APP_CONTINUE;
}
