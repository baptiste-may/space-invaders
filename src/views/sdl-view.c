#include "sdl-view.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window *win;
SDL_Renderer *rend;

void initViewSdl(Controller *controller) {
  if (SDL_Init(SDL_INIT_VIDEO) == false) {
    fprintf(stderr, "Cannot initialize SDL: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  win = SDL_CreateWindow("Space Invaders", WIDTH, HEIGHT, 0);
  if (win == NULL) {
    fprintf(stderr, "Cannot create window: %s", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  rend = SDL_CreateRenderer(win, NULL);
  if (rend == NULL) {
    fprintf(stderr, "Cannot create renderer: %s", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  SDL_RenderClear(rend);
  SDL_RenderPresent(rend);
}

void closeViewSdl() {
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
}

Event scanEventSdl(Controller *controller) {
  SDL_Event event;
  Event res = NO_EVENT;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      res = EVENT_CLOSE;
      break;
    case SDL_EVENT_KEY_DOWN:
      res = EVENT_CLOSE;
      break;
    }
  }
  SDL_Delay(100);
  return res;
}
