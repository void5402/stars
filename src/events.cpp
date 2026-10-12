#include "callbacks.hpp"

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  mystuff *stuff = static_cast<mystuff *>(appstate);
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_FAILURE;
  case SDL_EVENT_KEY_DOWN:
    if (event->key.key == SDLK_Q || event->key.key == SDLK_ESCAPE) {
      return SDL_APP_FAILURE;
    }
  }

  return SDL_APP_CONTINUE;
}