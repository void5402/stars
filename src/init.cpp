#include "callbacks.hpp"
#include <random>

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  Cluster *cluster = new Cluster;
  mystuff *stuff = new mystuff;

  SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "30");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("????", 1024, 1024,
                                   SDL_WINDOW_RESIZABLE |
                                       SDL_WINDOW_HIGH_PIXEL_DENSITY |
                                       SDL_WINDOW_MAXIMIZED,
                                   &(stuff->Window), &(stuff->Renderer))) {
    SDL_Log("Failed to make window or renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Log("Renderer backend: %s", SDL_GetRendererName(stuff->Renderer));
  SDL_GetWindowSizeInPixels(stuff->Window, &(stuff->w), &(stuff->h));
  // SDL_SetRenderVSync(stuff->Renderer, 1);

  static constmacro float l = maxradius;
  static constmacro float t = l / 10;
  static SDL_Vertex vert[12] = {
      {{l, 0.f}, {1.f, 1.f, 1.f, 0.f}, {0.f, 0.f}},
      {{l + t, l + t}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}},
      {{l - t, l + t}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}},
      {{l * 2, l}, {1.f, 1.f, 1.f, 0.f}, {0.f, 0.f}},
      {{l - t, l + t}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}},
      {{l - t, l - t}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}},
      {{l, l * 2}, {1.f, 1.f, 1.f, 0.f}, {0.f, 0.f}},
      {{l - t, l - t}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}},
      {{l + t, l - t}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}},
      {{0.f, l}, {1.f, 1.f, 1.f, 0.f}, {0.f, 0.f}},
      {{l + t, l - t}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}},
      {{l + t, l + t}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}}};

  SDL_Texture *texture = SDL_CreateTexture(
      stuff->Renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET,
      static_cast<int>(l * 2 + 1), static_cast<int>(l * 2 + 1));

  SDL_SetRenderTarget(stuff->Renderer, texture);
  SDL_SetRenderDrawColor(stuff->Renderer, 0, 0, 0, 0);
  SDL_RenderClear(stuff->Renderer);
  SDL_RenderGeometry(stuff->Renderer, NULL, vert, 12, NULL, 0);
  SDL_SetRenderTarget(stuff->Renderer, NULL);
  SDL_SetRenderDrawBlendMode(stuff->Renderer, SDL_BLENDMODE_ADD);

  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<float> xy(-maxz / 2.f, maxz / 2.f);
  std::uniform_real_distribution<float> z(1.f, maxz);
  for (size_t i = 0; i < n; i++) {
    cluster->x[i] = xy(rng);
    cluster->y[i] = xy(rng);
    cluster->z[i] = z(rng);
  }

  stuff->texture = texture;
  stuff->cluster = cluster;
  *appstate = stuff;

  return SDL_APP_CONTINUE;
}
