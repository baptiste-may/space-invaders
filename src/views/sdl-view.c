#include "sdl-view.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window *win;
SDL_Renderer *rend;

void initViewSdl() {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    perror("Cannot initialize SDL");
    exit(EXIT_FAILURE);
  }

  win = SDL_CreateWindow("Space Inviders", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  if (!win) {
    perror("Cannot create window");
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  rend = SDL_CreateRenderer(win, -1, 0);
  if (!rend) {
    perror("Cannot render window");
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
