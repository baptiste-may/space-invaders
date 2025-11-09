#ifndef VIEWS_H
#define VIEWS_H

#include "../model/model.h"

/**
 * @enum ViewType
 * @brief The type of view used to play the game
 *
 * @var ViewType NOT_DEFINED
 * @brief The view is not defined, the game cannot be launched
 *
 * @var ViewType NCURSES
 * @brief The app use Ncurses for the view
 *
 * @var ViewType SDL
 * @brief The app use SDL2 for the view
 */
typedef enum { NOT_DEFINED, NCURSES, SDL } ViewType;

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
void initView(ViewType viewType);

/**
 * @brief Close view depending on view type
 *
 * @param viewType The type of the view
 */
void closeView(ViewType viewType);

/**
 * @brief Loop depending on view type and model
 *
 * @param viewType The type of the view
 * @param model The model
 */
void loopView(ViewType viewType, Model *model);

#endif
