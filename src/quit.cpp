#include "callbacks.hpp"
#include <print>
extern double dt1, dt2, dt3, dt4, dt5, frames;
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    (void)result;
    mystuff *stuff = static_cast<mystuff *>(appstate);
    float * w= new float;
    float * h= new float;
    SDL_GetTextureSize(stuff->texture,w,h);
    std::print("w: {}, h: {}\n",*w,*h);
    SDL_DestroyTexture(stuff->texture);
    SDL_DestroyRenderer(stuff->Renderer);
    SDL_DestroyWindow(stuff->Window);

    std::print("dt1: {}, dt2: {}, dt2/n: {}, \ndt3: {}, dt4: {}, dt5: {}.\nfps: {}.\n",
               dt1 / frames, dt2 / frames, (dt2 / n) / frames, dt3 / frames, dt4 / frames, dt5 / frames,
               1000000 / ((dt1 + dt2 + dt3 + dt4 + dt5) / frames));

    SDL_DestroyTexture(stuff->texture);
    delete stuff->cluster;
    delete stuff;

    SDL_Quit();
}
