#pragma once

#include "views.h"

/**
 * @brief Initialize view using ncurses
 */
void initViewNcurses(Controller *controller);

/**
 * @brief Close view using ncurses
 */
void closeViewNcurses();

/**
 * @brief Scan the current event using ncurses
 *
 * @return The current event
 */
Event scanEventNcurses();

/**
 * @brief Create the main menu using ncurses
 *
 * @param controller The controller used
 */
void createMainMenuNcurses(Controller *controller);
/**
 * @brief Update the main menu using ncurses
 *
 * @param controller The controller used
 */
void updateMainMenuNcurses(Controller *controller);
/**
 * @brief Destroy the main menu using ncurses
 */
void destroyMainMenuNcurses();

/**
 * @brief Create the game window using ncurses
 *
 * @param controller The controller used
 */
void createGameNcurses(Controller *controller);
/**
 * @brief Update the game window using ncurses
 *
 * @param controller The controller used
 */
void updateGameNcurses(Controller *controller);
/**
 * @brief Destroy the game window using ncurses
 */
void destroyGameNcurses();

/**
 * @brief Size all windows using ncurses
 *
 * @param The controller used
 */
void resizeNcurses(Controller *controller);
