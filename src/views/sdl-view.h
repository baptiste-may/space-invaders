#pragma once

#include "../model/model.h"
#include "views.h"

#define WIDTH 1280
#define HEIGHT 720
#define FONT_SIZE 36

typedef enum { LEFT, CENTER, RIGHT } TextAlign;

/**
 * @brief Initialize view using sdl
 *
 * @param controller The controller used
 */
void initViewSdl(Controller *controller);

/**
 * @brief Close view using sdl
 */
void closeViewSdl();

/**
 * @brief Scan the current event using sdl
 *
 * @return The current event
 */
Event scanEventSdl();

/**
 * @brief Create main menu using sdl
 *
 * @param controller The controller used
 */
void createMainMenuSdl(Controller *controller);
/**
 * @brief Update main menu using sdl
 *
 * @param controller The controller used
 */
void updateMainMenuSdl(Controller *controller);
/**
 * @brief Destroy the main menu using sdl
 */
void destroyMainMenuSdl();

/**
 * @brief Create the game window using sdl
 *
 * @param controller The controller used
 */
void createGameSdl(Controller *controller);
/**
 * @brief Update the game window using sdl
 *
 * @param controller The controller used
 */
void updateGameSdl(Controller *controller);
/**
 * @brief Destroy the game window using sdl
 *
 * @param controller The controller used
 */
void destroyGameSdl();

/**
 * @brief Size all the window using sdl
 *
 * @param controller THe controller used
 */
void resizeSdl(Controller *controller);
