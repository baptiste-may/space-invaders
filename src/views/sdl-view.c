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
SDL_Texture *ufoTexture = NULL;
SDL_Texture *explosionTexture = NULL;

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

  // Load UFO texture
  ufoTexture = IMG_LoadTexture(rend, "assets/alien4.png");
  if (ufoTexture == NULL) {
    fprintf(stderr, "Cannot load UFO texture: %s", SDL_GetError());
    closeViewSdl();
    exit(EXIT_FAILURE);
  }
  SDL_SetTextureScaleMode(ufoTexture, SDL_SCALEMODE_NEAREST);

  explosionTexture = IMG_LoadTexture(rend, "assets/explosion.png");
  if (explosionTexture == NULL) {
    fprintf(stderr, "Cannot load explosion texture: %s", SDL_GetError());
    closeViewSdl();
    exit(EXIT_FAILURE);
  }
  SDL_SetTextureScaleMode(explosionTexture, SDL_SCALEMODE_NEAREST);

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
  if (ufoTexture != NULL)
    SDL_DestroyTexture(ufoTexture);
  if (explosionTexture != NULL)
    SDL_DestroyTexture(explosionTexture);
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
      res |= EVENT_CLOSE;
      break;
    case SDL_EVENT_WINDOW_RESIZED:
      res |= EVENT_RESIZE;
      break;
    case SDL_EVENT_KEY_DOWN:
      switch (event.key.key) {
      case SDLK_Z:
      case SDLK_UP:
        res |= EVENT_KEY_UP;
        break;
      case SDLK_S:
      case SDLK_DOWN:
        res |= EVENT_KEY_DOWN;
        break;
      case SDLK_RETURN:
        res |= EVENT_KEY_ENTER;
        break;
      case SDLK_ESCAPE:
        res |= EVENT_KEY_ESCAPE;
        break;
      }
      break;
    }
  }

  const bool *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_Q])
    res |= EVENT_KEY_LEFT;
  if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
    res |= EVENT_KEY_RIGHT;
  if (state[SDL_SCANCODE_SPACE])
    res |= EVENT_KEY_SPACE;

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

void renderShieldBlock(double x, double y, ShieldBlock block) {
  SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
  const double pixelSize = scale;
  bool pixels[2][2] = {{false, false}, {false, false}};
  switch (block) {
  case EMPTY:
    break;
  case FULL:
    pixels[0][0] = true;
    pixels[0][1] = true;
    pixels[1][0] = true;
    pixels[1][1] = true;
    break;
  case HALF_EMPTY_RIGHT:
    pixels[1][1] = true;
    break;
  case HALF_EMPTY_LEFT:
    pixels[1][0] = true;
    break;
  case HALF_FULL_RIGHT:
    pixels[0][0] = true;
    pixels[0][1] = true;
    pixels[1][0] = true;
    break;
  case HALF_FULL_LEFT:
    pixels[0][0] = true;
    pixels[0][1] = true;
    pixels[1][1] = true;
    break;
  case DESTROYED_RIGHT:
    pixels[0][1] = true;
    pixels[1][0] = true;
    break;
  case DESTROYED_LEFT:
    pixels[0][0] = true;
    pixels[1][1] = true;
  default:
    break;
  }

  SDL_FRect topLeft = {x, y, pixelSize, pixelSize};
  if (pixels[0][0])
    SDL_RenderFillRect(rend, &topLeft);
  SDL_FRect topRight = {x + pixelSize, y, pixelSize, pixelSize};
  if (pixels[0][1])
    SDL_RenderFillRect(rend, &topRight);
  SDL_FRect bottomLeft = {x, y + pixelSize, pixelSize, pixelSize};
  if (pixels[1][0])
    SDL_RenderFillRect(rend, &bottomLeft);
  SDL_FRect bottomRight = {x + pixelSize, y + pixelSize, pixelSize, pixelSize};
  if (pixels[1][1])
    SDL_RenderFillRect(rend, &bottomRight);
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
    sprintf(lives, "Lives: %d", game->player->lives);
    renderText(lives, width - 20, 30, white, RIGHT);

    // Header border
    SDL_FRect headerBorderRect = {0, (float)(height)*HEADER_HEIGHT_RATIO - 10,
                                  width, 2};
    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
    SDL_RenderFillRect(rend, &headerBorderRect);

    // Player
    float playerSizeX = 13 * scale;
    float playerSizeY = 8 * scale;
    // Align visual player position with logical position (including margin)
    float playerX = (game->player->position * GAME_WIDTH_RATIO +
                     (1.0 - GAME_WIDTH_RATIO) / 2.0) *
                        width -
                    playerSizeX / 2;
    SDL_FRect playerRect = {playerX, height * 0.975 - playerSizeY, playerSizeX,
                            playerSizeY};
    SDL_RenderTexture(rend, playerTexture, NULL, &playerRect);

    // Aliens
    const double gridHeight =
        height * ALIENS_HEIGHT_RATIO * ALIENS_GRID_HEIGHT_RATIO;
    const double moveRangeY =
        height * ALIENS_HEIGHT_RATIO * (1.0 - ALIENS_GRID_HEIGHT_RATIO);
    const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;

    for (unsigned i = 0; i < game->aliens->nbAlienRows; i++) {
      for (unsigned j = 0; j < game->aliens->nbAliens; j++) {
        unsigned k = j + i * game->aliens->nbAliens;
        int alienIndex = game->aliens->aliens[k];

        // Do not display dead aliens
        if (alienIndex < -EXPLOSION_FRAMES)
          continue;

        SDL_Texture *alienTexture =
            alienIndex < 0 ? explosionTexture
                           : aliensTextures[alienIndex / 2][alienIndex % 2];

        float alienSizeX, alienSizeY;
        SDL_GetTextureSize(alienTexture, &alienSizeX, &alienSizeY);
        alienSizeX *= scale, alienSizeY *= scale;

        double alienX =
            margin * width +
            ((width * GAME_WIDTH_RATIO) / (double)(game->aliens->nbAliens)) *
                (j + 0.5) +
            (game->aliens->aliensX - 0.5) * ((width * ALIENS_SWAY_FACTOR) /
                                             (double)(game->aliens->nbAliens));
        // CORRECTION: Les aliens commencent après l'UFO
        double alienY =
            (gridHeight / (double)(game->aliens->nbAlienRows)) * (i + 0.5) +
            height * (UFO_HEIGHT_RATIO + 0.05) +
            game->aliens->aliensY * moveRangeY;

        SDL_FRect alienRect = {alienX - alienSizeX / 2, alienY - alienSizeY / 2,
                               alienSizeX, alienSizeY};
        SDL_RenderTexture(rend, alienTexture, NULL, &alienRect);
      }
    }
    // UFO
    if (game->aliens->ufoActive) {
      float ufoSizeX, ufoSizeY;
      SDL_GetTextureSize(ufoTexture, &ufoSizeX, &ufoSizeY);
      ufoSizeX *= scale, ufoSizeY *= scale;

      double ufoX = game->aliens->ufoX * width;
      double ufoY = UFO_HEIGHT_RATIO * height;

      SDL_FRect ufoRect = {ufoX - ufoSizeX / 2, ufoY - ufoSizeY / 2, ufoSizeX,
                           ufoSizeY};
      SDL_RenderTexture(rend, ufoTexture, NULL, &ufoRect);
    }

    // Shields
    const double shieldsY =
        (1 - PLAYER_HEIGHT_RATIO - SHIELD_HEIGHT_RATIO / 2) * height;
    const double shieldsXDiff = (double)(width) / game->shields.nb;
    const double blockSize = 2 * scale;
    const double shieldWidth = SHIELD_WIDTH * blockSize;
    const double shieldHeight = SHIELD_HEIGHT * blockSize;

    for (unsigned n = 0; n < game->shields.nb; n++) {
      const double shieldX = shieldsXDiff * n + shieldsXDiff / 2;
      for (unsigned i = 0; i < SHIELD_HEIGHT; i++) {
        for (unsigned j = 0; j < SHIELD_WIDTH; j++) {
          ShieldBlock block = game->shields.blocks[n][i][j];
          if (block == EMPTY)
            continue;
          renderShieldBlock(shieldX - shieldWidth / 2 + j * blockSize,
                            shieldsY - shieldHeight / 2 + i * blockSize, block);
        }
      }
    }

    // Player shoot
    if (game->player->shootX >= 0 && game->player->shootY >= 0) {
      double shootY = game->player->shootY * height - 2 * scale;
      // Align visual shoot position with logical position
      SDL_FRect shootRect = {
          (game->player->shootX * GAME_WIDTH_RATIO + margin) * width -
              scale / 2,
          shootY, scale, 4 * scale};
      SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
      SDL_RenderFillRect(rend, &shootRect);
    }

    // Alien shots
    for (int i = 0; i < MAX_ALIEN_SHOTS; i++) {
      if (game->aliens->alienShotActive[i]) {
        // Align visual alien shoot position with logical position
        double alienShootX =
            (game->aliens->alienShotX[i] * GAME_WIDTH_RATIO + margin) * width -
            scale / 2;
        double alienShootY = game->aliens->alienShotY[i] * height - 2 * scale;
        SDL_FRect alienShootRect = {alienShootX, alienShootY, scale, 4 * scale};
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        SDL_RenderFillRect(rend, &alienShootRect);
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
