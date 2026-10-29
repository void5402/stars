#include "callbacks.hpp"
#include <SDL3/SDL_init.h>

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  mystuff *stuff = static_cast<mystuff *>(appstate);
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_DOWN:
    if (event->key.key == SDLK_Q || event->key.key == SDLK_ESCAPE) {
      return SDL_APP_SUCCESS;
    }
  }

  return SDL_APP_CONTINUE;
}