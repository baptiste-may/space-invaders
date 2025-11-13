#ifndef VIEWS_H
#define VIEWS_H

#include "../controller/controller.h"

/**
 * @enum ViewType
 * @brief The type of view used to play the game
 */
typedef enum {
  NCURSES, //!< The app use Ncurses for the view
  SDL      //!< The app use SDL3 for the view
} ViewType;

/**
 * @brief Return the correct enum depending on the string argument
 *
 * @param arg The string argument
 * @return The view type
 */
ViewType getViewType(const char *arg);

/**
 * @brief Initialize view depending on the controller
 *
 * @param controller The controller used
 */
void initView(Controller *controller);

/**
 * @brief Close view depending on view type
 *
 * @param viewType The type of the view
 */
void closeView(ViewType viewType);

/**
 * @brief Scan the current event depending on the view type
 *
 * @param viewType The type of the view
 * @return The current event
 */
Event scanEvent(ViewType viewType);

/**
 * @brief Create the main menu depending on the controller
 *
 * @param controller The controller used
 */
void createMainMenu(Controller *controller);
/**
 * @brief Update the main menu depending on the controller
 *
 * @param controller The controller used
 */
void updateMainMenu(Controller *controller);
/**
 * @brief Destroy the main menu depending on the view type
 *
 * @param viewType The type of the view
 */
void destroyMainMenu(ViewType viewType);

/**
 * @brief Create game window depending on the controller
 *
 * @param controller The controller used
 */
void createGame(Controller *controller);
/**
 * @brief Update the game window depending on the controller
 *
 * @param controller The controller used
 */
void updateGame(Controller *controller);
/**
 * @brief Destroy the game window depending on the view type
 *
 * @param viewType The type of the view
 */
void destroyGame(ViewType viewType);

/**
 * @brief Resize all windows depending on the controller
 *
 * @param controller The controller used
 */
void resize(Controller *controller);

#endif
