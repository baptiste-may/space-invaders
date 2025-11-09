#ifndef NCURSES_VIEW_H
#define NCURSES_VIEW_H

#include "../model/model.h"

/**
 * @brief Initialize view using ncurses
 */
void initViewNcurses();

/**
 * @brief Close view using ncurses
 */
void closeViewNcurses();

/**
 * @brief Loop view using ncurses
 */
void loopViewNcurses(Model *model);

#endif
