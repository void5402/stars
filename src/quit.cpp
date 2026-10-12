#include "callbacks.hpp"

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    mystuff *stuff = static_cast<mystuff *>(appstate);
    SDL_DestroyWindow(stuff->Wnindow);
    SDL_DestroyRenderer(stuff->Renderer);
    delete stuff;
    SDL_Quit();
}