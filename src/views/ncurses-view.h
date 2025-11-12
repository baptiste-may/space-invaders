#ifndef NCURSES_VIEW_H
#define NCURSES_VIEW_H

#include "views.h"

/**
 * @brief Initialize view using ncurses
 */
void initViewNcurses(Controller *controller);

/**
 * @brief Close view using ncurses
 */
void closeViewNcurses();

Event scanEventNcurses(Controller *controller);

void createMainMenuNcurses(Controller *controller);
void updateMainMenuNcurses(Controller *controller);
void destroyMainMenuNcurses();

void createGameNcurses(Controller *controller);
void updateGameNcurses(Controller *controller);
void destroyGameNcurses();

void resizeNcurses(Controller *controller);

#endif
