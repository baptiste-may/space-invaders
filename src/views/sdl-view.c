#include "sdl-view.h"
#include "views.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int width, height;
double scale = 1;

SDL_Window *win = NULL;
SDL_Renderer *rend = NULL;
TTF_Font *font = NULL;
SDL_Texture *playerTexture = NULL;
SDL_Texture *aliensTextures[4][2];

SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

void initViewSdl(Controller *controller) {
  if (SDL_Init(SDL_INIT_VIDEO) == false) {
    fprintf(stderr, "Cannot initialize SDL: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  win = SDL_CreateWindow("Space Invaders", WIDTH, HEIGHT, 0);
  if (win == NULL) {
    fprintf(stderr, "Cannot create window: %s", SDL_GetError());
    closeViewSdl();
    exit(EXIT_FAILURE);
  }

  rend = SDL_CreateRenderer(win, NULL);
  if (rend == NULL) {
    fprintf(stderr, "Cannot create renderer: %s", SDL_GetError());
    closeViewSdl();
    exit(EXIT_FAILURE);
  }

  // Load player texture
  playerTexture = IMG_LoadTexture(rend, "assets/player.png");
  if (playerTexture == NULL) {
    fprintf(stderr, "Cannot load player texture: %s", SDL_GetError());
    closeViewSdl();
    exit(EXIT_FAILURE);
  }
  SDL_SetTextureScaleMode(playerTexture, SDL_SCALEMODE_NEAREST);

  // Load aliens textures
  for (unsigned i = 1; i <= 4; i++) {
    char imagePath[20];
    for (unsigned j = 1; j <= 2; j++) {
      if (i != 4)
        sprintf(imagePath, "assets/alien%u-%u.png", i, j);
      else
        sprintf(imagePath, "assets/alien4.png");
      aliensTextures[i - 1][j - 1] = IMG_LoadTexture(rend, imagePath);
      if (aliensTextures[i - 1][j - 1] == NULL) {
        fprintf(stderr, "Cannot load alien n°%u with texture n°%u: %s", i, j,
                SDL_GetError());
        closeViewSdl();
        exit(EXIT_FAILURE);
      }
      SDL_SetTextureScaleMode(aliensTextures[i - 1][j - 1],
                              SDL_SCALEMODE_NEAREST);
    }
  }

  if (TTF_Init() == false) {
    fprintf(stderr, "Cannot initialize TTF: %s", SDL_GetError());
    closeViewSdl();
    exit(EXIT_FAILURE);
  }

  font = TTF_OpenFont("assets/editundo.ttf", FONT_SIZE);
  if (font == NULL) {
    fprintf(stderr, "Cannot load font: %s", SDL_GetError());
    closeViewSdl();
    exit(EXIT_FAILURE);
  }

  SDL_RenderClear(rend);
  SDL_RenderPresent(rend);

  resize(controller);

  createMainMenuSdl(controller);
}

void closeViewSdl() {
  if (font != NULL)
    TTF_CloseFont(font);
  TTF_Quit();
  if (playerTexture != NULL)
    SDL_DestroyTexture(playerTexture);
  for (unsigned i = 0; i < 4; i++) {
    for (unsigned j = 0; j < 2; j++) {
      if (aliensTextures[i][j] != NULL)
        SDL_DestroyTexture(aliensTextures[i][j]);
    }
  }
  if (rend != NULL)
    SDL_DestroyRenderer(rend);
  if (win != NULL)
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
      case SDLK_Z:
      case SDLK_UP:
        res = EVENT_KEY_UP;
        break;
      case SDLK_S:
      case SDLK_DOWN:
        res = EVENT_KEY_DOWN;
        break;
      case SDLK_Q:
      case SDLK_LEFT:
        res = EVENT_KEY_LEFT;
        break;
      case SDLK_D:
      case SDLK_RIGHT:
        res = EVENT_KEY_RIGHT;
        break;
      case SDLK_RETURN:
        res = EVENT_KEY_ENTER;
        break;
      case SDLK_ESCAPE:
        res = EVENT_KEY_ESCAPE;
        break;
      case SDLK_SPACE:
        res = EVENT_KEY_SPACE;
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
  if (textAlign == LEFT)
    finalX = x;
  else if (textAlign == RIGHT)
    finalX = x - (float)text_width;
  else
    finalX = x - (float)text_width / 2;

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

    // Score
    char score[20];
    sprintf(score, "Score: %d", game->score);
    renderText(score, 20, 30, white, LEFT);

    // Lives
    char lives[20];
    sprintf(lives, "Lives: %d", game->lives);
    renderText(lives, width - 20, 30, white, RIGHT);

    // Header border
    SDL_FRect headerBorderRect = {0, (float)(height) * HEADER_HEIGHT_RATIO - 10,
                                  width, 2};
    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
    SDL_RenderFillRect(rend, &headerBorderRect);

    // Player
    float playerSizeX = 13 * scale;
    float playerSizeY = 8 * scale;
    float playerX = game->playerPosition * (width - playerSizeX * 2) +
                    (float)(playerSizeX) / 2;
    SDL_FRect playerRect = {playerX, height * 0.975 - playerSizeY, playerSizeX,
                            playerSizeY};
    SDL_RenderTexture(rend, playerTexture, NULL, &playerRect);

    // Player shoot
    if (game->playerShootX >= 0 && game->playerShootY >= 0) {
      double shootY = game->playerShootY * height - 2 * scale;
      SDL_FRect shootRect = {game->playerShootX * (width - playerSizeX * 2) +
                                 playerSizeX - scale / 2,
                             shootY, scale, 4 * scale};
      SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
      SDL_RenderFillRect(rend, &shootRect);
    }

    // Alien shots
    for (int i = 0; i < MAX_ALIEN_SHOTS; i++) {
      if (game->alienShotActive[i]) {
        double alienShootX = game->alienShotX[i] * (width - playerSizeX * 2) + 
                             playerSizeX - scale / 2;
        double alienShootY = game->alienShotY[i] * height - 2 * scale;
        SDL_FRect alienShootRect = {alienShootX, alienShootY, scale, 4 * scale};
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); // Rouge pour les tirs aliens
        SDL_RenderFillRect(rend, &alienShootRect);
      }
    }

    // Aliens
    const double gridHeight =
        height * ALIENS_HEIGHT_RATIO * ALIENS_GRID_HEIGHT_RATIO;
    const double moveRangeY =
        height * ALIENS_HEIGHT_RATIO * (1.0 - ALIENS_GRID_HEIGHT_RATIO);

    for (unsigned i = 0; i < game->nbAlienRows; i++) {
      for (unsigned j = 0; j < game->nbAliens; j++) {
        unsigned k = j + i * game->nbAliens;
        int alienIndex = game->aliens[k];

        // Ne pas afficher les aliens morts
        if (alienIndex < 0)
          continue;

        SDL_Texture *alienTexture = aliensTextures[alienIndex / 2][0];

        float alienSizeX, alienSizeY;
        SDL_GetTextureSize(alienTexture, &alienSizeX, &alienSizeY);
        alienSizeX *= scale, alienSizeY *= scale;

        double alienX =
            ((width * GAME_WIDTH_RATIO) / (double)(game->nbAliens)) *
                (j + 0.5) +
            game->aliensX *
                ((width * ALIENS_SWAY_FACTOR) / (double)(game->nbAliens));
        double alienY =
            (gridHeight / (double)(game->nbAlienRows)) * (i + 0.5) +
            height * HEADER_HEIGHT_RATIO + game->aliensY * moveRangeY;

        SDL_FRect alienRect = {alienX - alienSizeX / 2, alienY - alienSizeY / 2,
                               alienSizeX, alienSizeY};
        SDL_RenderTexture(rend, aliensTextures[alienIndex / 2][alienIndex % 2],
                          NULL, &alienRect);
      }
    }
  }

  SDL_RenderPresent(rend);
}

void destroyGameSdl() {}

void resizeSdl(Controller *controller) {
  SDL_GetWindowSize(win, &width, &height);
  scale = (double)(width) / 640 + (double)(height) / 480;
}
