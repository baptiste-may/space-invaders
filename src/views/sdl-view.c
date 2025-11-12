#include "sdl-view.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window *win;
SDL_Renderer *rend;

void initViewSdl() {
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

void loopViewSdl(Model *model) { SDL_Delay(5000); }
