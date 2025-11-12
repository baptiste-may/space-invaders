#ifndef SDL_VIEW_H
#define SDL_VIEW_H

#include "../model/model.h"
#include "views.h"

#define WIDTH 600
#define HEIGHT 400

/**
 * @brief Initialize view using sdl
 */
void initViewSdl(Controller *controller);

/**
 * @brief Close view using sdl
 */
void closeViewSdl();

Event scanEventSdl(Controller *controller);

void createMainMenuSdl(Controller *controller);
void updateMainMenuSdl(Controller *controller);
void destroyMainMenuSdl();

void createGameSdl(Controller *controller);
void updateGameSdl(Controller *controller);
void destroyGameSdl();

void resizeSdl(Controller *controller);

#endif
