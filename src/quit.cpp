#include "callbacks.hpp"

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    mystuff *stuff = static_cast<mystuff *>(appstate);
    (void)result;
    
    SDL_DestroyWindow(stuff->Window);
    SDL_DestroyRenderer(stuff->Renderer);
    delete[] stuff;
    
    SDL_Quit();
}