#include "callbacks.hpp"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
  (void)argc;
  (void)argv;
  // SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "60");
  // SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland");

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  Cluster *cluster = new Cluster;
  mystuff *stuff = new mystuff;
  if (!SDL_CreateWindowAndRenderer("????", 512, 512, SDL_WINDOW_VULKAN, &(stuff->Window), &(stuff->Renderer)))
  {
    SDL_Log("Failed to make window or renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_GetWindowSizeInPixels(stuff->Window, &(stuff->w), &(stuff->h));
  SDL_SetRenderVSync(stuff->Renderer,1);

  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<float> xy(-maxz / 2.f, maxz / 2.f);
  std::uniform_real_distribution<float> z(1.f, maxz);
  for (size_t i = 0; i < n; i++)
  {
    cluster->x[i] = xy(rng);
    cluster->y[i] = xy(rng);
    cluster->z[i] = z(rng);
  }


  stuff->cluster = cluster;
  *appstate = stuff;

  return SDL_APP_CONTINUE;
}