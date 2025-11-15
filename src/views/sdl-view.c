#include "sdl-view.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int width, height;

SDL_Window *win;
SDL_Renderer *rend;
TTF_Font *font;

SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

void initViewSdl(Controller *controller) {
  if (SDL_Init(SDL_INIT_VIDEO) == false) {
    fprintf(stderr, "Cannot initialize SDL: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  width = WIDTH;
  height = HEIGHT;

  win = SDL_CreateWindow("Space Invaders", width, height, 0);
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

  if (TTF_Init() == false) {
    fprintf(stderr, "Cannot initialize TTF: %s", SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  font = TTF_OpenFont("assets/editundo.ttf", FONT_SIZE);
  if (font == NULL) {
    fprintf(stderr, "Cannot load font: %s", SDL_GetError());
    TTF_Quit();
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  SDL_RenderClear(rend);
  SDL_RenderPresent(rend);

  createMainMenuSdl(controller);
}

void closeViewSdl() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
}

Event scanEventSdl() {
  SDL_Event event;
  Event res = NO_EVENT;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      res = EVENT_CLOSE;
      break;
    case SDL_EVENT_WINDOW_RESIZED:
      res = EVENT_RESIZE;
      break;
    case SDL_EVENT_KEY_DOWN:
      switch (event.key.key) {
      case SDLK_UP:
        res = EVENT_KEY_UP;
        break;
      case SDLK_DOWN:
        res = EVENT_KEY_DOWN;
        break;
      case SDLK_RETURN:
        res = EVENT_KEY_ENTER;
        break;
      case SDLK_ESCAPE:
        res = EVENT_KEY_ESCAPE;
        break;
      }
      break;
    }
  }
  SDL_Delay(10);
  return res;
}

void renderText(const char *text, float x, float y, SDL_Color color,
                TextAlign textAlign) {
  int text_width, text_height;
  TTF_GetStringSize(font, text, 0, &text_width, &text_height);

  SDL_Surface *surface = TTF_RenderText_Solid(font, text, strlen(text), color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surface);

  float finalX;
  if (textAlign == LEFT) {
    finalX = x;
  } else if (textAlign == RIGHT) {
    finalX = x - (float)text_width;
  } else {
    finalX = x - (float)text_width / 2;
  }

  SDL_FRect rect = {finalX, y - (float)text_height / 2, (float)surface->w,
                    (float)surface->h};
  SDL_RenderTexture(rend, texture, NULL, &rect);
  SDL_DestroyTexture(texture);
  SDL_DestroySurface(surface);
}

void createMainMenuSdl(Controller *controller) {
  updateMainMenuSdl(controller);
}

void updateMainMenuSdl(Controller *controller) {
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_RenderClear(rend);

  Model *model = controller->model;
  int selected = model->mainMenu.selected;

  renderText("Space Invaders", (float)(width) / 2, 50, white, CENTER);

  renderText(model->currentGame == NULL ? "Play" : "Continue",
             (float)(width) / 2, 150,
             selected == 0 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);
  renderText("Settings", (float)(width) / 2, 200,
             selected == 1 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);
  renderText("Credit", (float)(width) / 2, 250,
             selected == 2 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);
  renderText("Quit", (float)(width) / 2, 300,
             selected == 3 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);

  SDL_RenderPresent(rend);
}

void destroyMainMenuSdl() {}

void createGameSdl(Controller *controller) { updateGameSdl(controller); }

void updateGameSdl(Controller *controller) {
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_RenderClear(rend);

  Model *model = controller->model;

  if (model->currentGame != NULL) {
    Game *game = model->currentGame;
    char score[20];
    sprintf(score, "Score: %d", game->score);
    renderText(score, 20, 20, white, LEFT);

    char lives[20];
    sprintf(lives, "Lives: %d", game->lives);
    renderText(lives, width - 20, 20, white, RIGHT);
  }

  SDL_RenderPresent(rend);
}

void destroyGameSdl() {}

void resizeSdl(Controller *controller) {
  SDL_GetWindowSize(win, &width, &height);
}
