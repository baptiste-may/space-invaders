#ifndef VIEWS_H
#define VIEWS_H

#include "../controller/controller.h"

/**
 * @enum ViewType
 * @brief The type of view used to play the game
 *
 * @var ViewType NCURSES
 * @brief The app use Ncurses for the view
 *
 * @var ViewType SDL
 * @brief The app use SDL2 for the view
 */
typedef enum { NCURSES, SDL } ViewType;

/**
 * @brief Return the correct enum depending on the string argument
 *
 * @param arg The string argument
 */
ViewType getViewType(const char *arg);

/**
 * @brief Initialize view depending on view type
 *
 * @param viewType The type of the view
 */
void initView(Controller *controller);

/**
 * @brief Close view depending on view type
 *
 * @param viewType The type of the view
 */
void closeView(ViewType viewType);

Event scanEvent(Controller *controller);

void createMainMenu(Controller *controller);
void updateMainMenu(Controller *controller);
void destroyMainMenu(ViewType viewType);

void createGame(Controller *controller);
void updateGame(Controller *controller);
void destroyGame(ViewType viewType);

void resize(Controller *controller);

#endif
