#include "sdl-view.h"
#include "../controller/controller.h"
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
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 1280
#define HEIGHT 720
#define FONT_SIZE 36

typedef enum { LEFT, CENTER, RIGHT } TextAlign;

int width, height;
double scale = 1;

SDL_Window *win = NULL;
SDL_Renderer *rend = NULL;
TTF_Font *font = NULL;
SDL_Texture *playerTexture = NULL;
SDL_Texture *aliensTextures[4][2];
SDL_Texture *ufoTexture = NULL;
SDL_Texture *explosionTexture = NULL;
SDL_Texture *shootTextures[NB_SHOOT_SPRITE];

SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

/**
 * @brief Render the game elements (player, aliens, etc.)
 *
 * @param controller The controller containing the model
 */
static void renderGameElementsSdl(Controller *controller);

/**
 * @brief Close the SDL view and cleanup
 */
static void closeViewSdl() {
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

/**
 * @brief Handle resize events for SDL
 *
 * @param controller The controller
 */
static void resizeSdl(Controller *controller) {
  SDL_GetWindowSize(win, &width, &height);
  scale = (double)(width) / 640 + (double)(height) / 480;
}

/**
 * @brief Render text on the screen
 *
 * @param text The text to render
 * @param x The x coordinate
 * @param y The y coordinate
 * @param color The color of the text
 * @param textAlign The alignment of the text
 */
static void renderText(const char *text, float x, float y, SDL_Color color,
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

/**
 * @brief Render a box with an outline
 *
 * @param boxWidth Width of the box
 * @param boxHeight Height of the box
 * @param boxX X coordinate of the top-left corner
 * @param boxY Y coordinate of the top-left corner
 * @param boxOutline Thickness of the outline
 */
static void renderBox(float boxWidth, float boxHeight, float boxX, float boxY,
                      float boxOutline) {
  SDL_FRect bgOutline = {boxX - boxOutline, boxY - boxOutline,
                         boxWidth + boxOutline * 2, boxHeight + boxOutline * 2};
  SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
  SDL_RenderFillRect(rend, &bgOutline);
  SDL_FRect bg = {boxX, boxY, boxWidth, boxHeight};
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_RenderFillRect(rend, &bg);
}

/**
 * @brief Update and draw the main menu
 *
 * @param controller The controller containing the model
 */
static void updateMainMenuSdl(Controller *controller) {
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_RenderClear(rend);

  renderGameElementsSdl(controller);

  Model *model = controller->model;
  int selected = model->mainMenu.selected;

  double winWidth = 400, winHeight = 350;
  float boxX = (float)(width - winWidth) / 2;
  float boxY = (float)(height - winHeight) / 2;
  renderBox(winWidth, winHeight, boxX, boxY, 2);

  renderText("Space Invaders", boxX + (float)winWidth / 2, boxY + 50, white,
             CENTER);

  renderText(model->currentGame == NULL ? "Play" : "Continue",
             boxX + (float)winWidth / 2, boxY + 150,
             selected == 0 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);
  renderText("Credit", boxX + (float)winWidth / 2, boxY + 200,
             selected == 1 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);
  renderText("Quit", boxX + (float)winWidth / 2, boxY + 250,
             selected == 2 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);

  SDL_RenderPresent(rend);
}

/**
 * @brief Update and draw the credits menu
 *
 * @param controller The controller containing the model
 */
static void updateCreditsMenuSdl(Controller *controller) {
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_RenderClear(rend);

  renderGameElementsSdl(controller);

  Model *model = controller->model;
  int selected = model->creditsMenu.selected;

  double winWidth = 400, winHeight = 350;
  float boxX = (float)(width - winWidth) / 2;
  float boxY = (float)(height - winHeight) / 2;
  renderBox(winWidth, winHeight, boxX, boxY, 2);

  renderText("Credits", boxX + (float)winWidth / 2, boxY + 40, white, CENTER);

  renderText("Made by:", boxX + (float)winWidth / 2, boxY + 90, white, CENTER);
  renderText("Baptiste MAY", boxX + (float)winWidth / 2, boxY + 130, white,
             CENTER);
  renderText("Kamil CHARBENAGA", boxX + (float)winWidth / 2, boxY + 170, white,
             CENTER);

  renderText("See on github", boxX + (float)winWidth / 2, boxY + 240,
             selected == 0 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);

  renderText("Exit", boxX + (float)winWidth / 2, boxY + 290,
             selected == 1 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);

  SDL_RenderPresent(rend);
}

/**
 * @brief Update and draw the game over menu
 *
 * @param controller The controller containing the model
 */
static void updateGameOverMenuSdl(Controller *controller) {
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_RenderClear(rend);

  renderGameElementsSdl(controller);

  Game *game = controller->model->currentGame;
  if (game == NULL)
    return;

  double winWidth = 400, winHeight = 350;
  float boxX = (float)(width - winWidth) / 2;
  float boxY = (float)(height - winHeight) / 2;
  renderBox(winWidth, winHeight, boxX, boxY, 2);

  renderText("GAME OVER", boxX + (float)winWidth / 2, boxY + 50, white, CENTER);

  char finalScore[64];
  snprintf(finalScore, sizeof(finalScore), "Final Score: %d",
           game->scores.current);
  renderText(finalScore, boxX + (float)winWidth / 2, boxY + 120, white, CENTER);

  char bestScore[64];
  snprintf(bestScore, sizeof(bestScore), "Best Score: %d", game->scores.best);
  renderText(bestScore, boxX + (float)winWidth / 2, boxY + 170,
             (SDL_Color){128, 128, 128, 255}, CENTER);

  // Menu options
  int selected = controller->model->gameOverMenu.selected;

  renderText("Restart", boxX + (float)winWidth / 2, boxY + 240,
             selected == 0 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);
  renderText("Main Menu", boxX + (float)winWidth / 2, boxY + 290,
             selected == 1 ? white : (SDL_Color){128, 128, 128, 255}, CENTER);

  SDL_RenderPresent(rend);
}

/**
 * @brief Create the main menu (wrapper for update)
 *
 * @param controller The controller containing the model
 */
static void createMainMenuSdl(Controller *controller) {
  updateMainMenuSdl(controller);
}

/**
 * @brief Create the credits menu (wrapper for update)
 *
 * @param controller The controller containing the model
 */
static void createCreditsMenuSdl(Controller *controller) {
  updateCreditsMenuSdl(controller);
}

/**
 * @brief Create the game over menu (wrapper for update)
 *
 * @param controller The controller containing the model
 */
static void createGameOverMenuSdl(Controller *controller) {
  updateGameOverMenuSdl(controller);
}

/**
 * @brief Render a single shield block
 *
 * @param x X coordinate
 * @param y Y coordinate
 * @param block The block type to render
 */
static void renderShieldBlock(double x, double y, ShieldBlock block) {
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

/**
 * @brief Render the game elements (player, aliens, etc.)
 *
 * @param controller The controller containing the model
 */
static void renderGameElementsSdl(Controller *controller) {
  Model *model = controller->model;

  if (model->currentGame != NULL) {
    Game *game = model->currentGame;

    // All logical coordinates (0.0 to 1.0) must be scaled to screen dimensions (width, height).
    // GAME_WIDTH_RATIO handles the horizontal padding of the game area.

    // Just try a buffer instead of dynamics, for score, best score and live.
    // Score
    char score[64];
    snprintf(score, sizeof(score), "Score: %d", game->scores.current);
    renderText(score, 20, 30, white, LEFT);

    // Best score
    unsigned bestScoreSize =
        12 + (unsigned)(floor(log10(game->scores.best)) + 1);
    char bestScore[64];
    snprintf(bestScore, sizeof(bestScore), "Best score: %d", game->scores.best);
    renderText(bestScore, width / 2. - bestScoreSize / 2., 30, white, CENTER);

    // Lives
    char lives[64];
    snprintf(lives, sizeof(lives), "Lives: %d", game->player->lives);
    renderText(lives, width - 20, 30, white, RIGHT);

    // Header border
    SDL_FRect headerBorderRect = {0, (float)(height)*HEADER_HEIGHT_RATIO - 10,
                                  width, 2};
    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
    SDL_RenderFillRect(rend, &headerBorderRect);

    // Player - with death animation
    if (game->playerDeathFrame >= 0 &&
        game->playerDeathFrame < PLAYER_DEATH_FRAMES) {
      // Show explosion for first half of animation
      if (game->playerDeathFrame < PLAYER_DEATH_FRAMES / 2) {
        float playerSizeX = 13 * scale;
        float playerSizeY = 8 * scale;
        float playerX = (game->player->position * GAME_WIDTH_RATIO +
                         (1.0 - GAME_WIDTH_RATIO) / 2.0) *
                            width -
                        playerSizeX / 2;
        SDL_FRect explosionRect = {playerX, height * 0.975 - playerSizeY,
                                   playerSizeX, playerSizeY};
        SDL_RenderTexture(rend, explosionTexture, NULL, &explosionRect);
      }
      // Second half is empty (pause)
    } else {
      // Normal player rendering
      float playerSizeX = 13 * scale;
      float playerSizeY = 8 * scale;
      float playerX = (game->player->position * GAME_WIDTH_RATIO +
                       (1.0 - GAME_WIDTH_RATIO) / 2.0) *
                          width -
                      playerSizeX / 2;
      SDL_FRect playerRect = {playerX, height * 0.975 - playerSizeY,
                              playerSizeX, playerSizeY};
      SDL_RenderTexture(rend, playerTexture, NULL, &playerRect);
    }

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
        // FIX: Aliens start below the UFO
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

    // Alien shoots
    for (int i = 0; i < MAX_ALIEN_SHOOTS; i++) {
      if (game->aliens->alienShootStatus[i]) {
        // Align visual alien shoot position with logical position
        double alienShootX =
            (game->aliens->alienShootX[i] * GAME_WIDTH_RATIO + margin) * width;
        double alienShootY = game->aliens->alienShootY[i] * height;

        SDL_Texture *shootTexture =
            shootTextures[game->aliens->alienShootStatus[i] - 1];
        float shootSizeX, shootSizeY;
        SDL_GetTextureSize(shootTexture, &shootSizeX, &shootSizeY);
        shootSizeX *= scale;
        shootSizeY *= scale;

        SDL_FRect alienShootRect = {alienShootX - shootSizeX / 2,
                                    alienShootY - shootSizeY / 2, shootSizeX,
                                    shootSizeY};
        SDL_RenderTexture(rend, shootTexture, NULL, &alienShootRect);
      }
    }
  }
}

/**
 * @brief Update and draw the game
 *
 * @param controller The controller containing the model
 */
static void updateGameSdl(Controller *controller) {
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_RenderClear(rend);
  renderGameElementsSdl(controller);
  SDL_RenderPresent(rend);
}

/**
 * @brief Create the game (wrapper for update)
 *
 * @param controller The controller containing the model
 */
static void createGameSdl(Controller *controller) { updateGameSdl(controller); }

/**
 * @brief Destroy the main menu (no-op in SDL)
 */
static void destroyMainMenuSdl() {}
/**
 * @brief Destroy the credits menu (no-op in SDL)
 */
static void destroyCreditsMenuSdl() {}
/**
 * @brief Destroy the game over menu (no-op in SDL)
 */
static void destroyGameOverMenuSdl() {}
/**
 * @brief Destroy the game (no-op in SDL)
 */
static void destroyGameSdl() {}

/**
 * @brief Check for SDL errors and exit if one occurs
 *
 * @param obj The object to check (NULL indicates error)
 * @param errMsg The error message to print
 */
static void checkSdlError(void *obj, const char *errMsg) {
  if (obj == NULL) {
    fprintf(stderr, "%s: %s", errMsg, SDL_GetError());
    closeViewSdl();
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Initialize the SDL view
 *
 * @param controller The controller
 */
static void initViewSdl(Controller *controller) {
  if (SDL_Init(SDL_INIT_VIDEO) == false) {
    fprintf(stderr, "Cannot initialize SDL: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  // Create main window
  win = SDL_CreateWindow("Space Invaders", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
  checkSdlError(win, "Cannot create window");

  // Create render based on the main window
  rend = SDL_CreateRenderer(win, NULL);
  checkSdlError(rend, "Cannot create renderer");

  // --- Texture Loading ---

  // Load player texture
  playerTexture = IMG_LoadTexture(rend, "assets/player.png");
  checkSdlError(playerTexture, "Cannot load player texture");
  SDL_SetTextureScaleMode(playerTexture, SDL_SCALEMODE_NEAREST);

  // Load aliens textures
  for (unsigned i = 1; i <= 4; i++) {
    char imagePath[20];
    for (unsigned j = 1; j <= 2; j++) {
      if (i != 4)
        sprintf(imagePath, "assets/alien%u-%u.png", i, j);
      else
        sprintf(imagePath, "assets/alien4.png");
      SDL_Texture *texture = IMG_LoadTexture(rend, imagePath);
      char errorMsg[50];
      sprintf(errorMsg, "Cannot load alien n°%u with texture n°%u", i, j);
      checkSdlError(texture, errorMsg);
      SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
      aliensTextures[i - 1][j - 1] = texture;
    }
  }

  // Load UFO texture
  ufoTexture = IMG_LoadTexture(rend, "assets/alien4.png");
  checkSdlError(ufoTexture, "Cannot load UFO texture");
  SDL_SetTextureScaleMode(ufoTexture, SDL_SCALEMODE_NEAREST);

  // Load explosion texture
  explosionTexture = IMG_LoadTexture(rend, "assets/explosion.png");
  checkSdlError(explosionTexture, "Cannot load explosion texture");
  SDL_SetTextureScaleMode(explosionTexture, SDL_SCALEMODE_NEAREST);

  // Load shoot textures
  for (unsigned i = 1; i <= NB_SHOOT_SPRITE; i++) {
    char imgPath[20];
    sprintf(imgPath, "assets/shoot-%u.png", i);
    SDL_Texture *texture = IMG_LoadTexture(rend, imgPath);
    char errorMsg[50];
    sprintf(errorMsg, "Cannot load shoot texture n°%u", i);
    checkSdlError(texture, errorMsg);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    shootTextures[i - 1] = texture;
  }

  // --- Font Initialization ---

  // Load ttf module (for fonts)
  if (TTF_Init() == false) {
    fprintf(stderr, "Cannot initialize TTF: %s", SDL_GetError());
    closeViewSdl();
    exit(EXIT_FAILURE);
  }

  // Load editundo font
  font = TTF_OpenFont("assets/editundo.ttf", FONT_SIZE);
  checkSdlError(font, "Cannot load font");

  SDL_RenderClear(rend);
  SDL_RenderPresent(rend);

  resizeSdl(controller);

  createMainMenuSdl(controller);
}

/**
 * @brief Scan for user input events in SDL
 *
 * @return The event detected
 */
static Event scanEventSdl() {
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

ViewInterface getSdlInterface() {
  return (ViewInterface){initViewSdl,
                         closeViewSdl,
                         scanEventSdl,
                         createMainMenuSdl,
                         updateMainMenuSdl,
                         destroyMainMenuSdl,
                         createCreditsMenuSdl,
                         updateCreditsMenuSdl,
                         destroyCreditsMenuSdl,
                         createGameSdl,
                         updateGameSdl,
                         destroyGameSdl,
                         createGameOverMenuSdl,
                         updateGameOverMenuSdl,
                         destroyGameOverMenuSdl,
                         resizeSdl};
}
