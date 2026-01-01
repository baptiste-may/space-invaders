#pragma once

#include "../controller/controller.h"

/**
 * @enum ViewType
 * @brief The type of view used to play the game
 */
typedef enum {
  NCURSES, //!< The app uses Ncurses for the view
  SDL      //!< The app uses SDL3 for the view
} ViewType;

/**
 * @struct ViewInterface
 * @brief Interface for view implementations
 */
typedef struct ViewInterface {
  void (*init)(Controller *controller);
  void (*close)();
  Event (*scanEvent)();
  void (*createMainMenu)(Controller *controller);
  void (*updateMainMenu)(Controller *controller);
  void (*destroyMainMenu)();
  void (*createGame)(Controller *controller);
  void (*updateGame)(Controller *controller);
  void (*destroyGame)();
  void (*createGameOverMenu)(Controller *controller);
  void (*updateGameOverMenu)(Controller *controller);
  void (*destroyGameOverMenu)();
  void (*resize)(Controller *controller);
} ViewInterface;

/**
 * @brief Return the correct enum depending on the string argument
 *
 * @param arg The string argument
 * @return The view type
 */
ViewType getViewType(const char *arg);

/**
 * @brief Initialize view and create controller
 *
 * @param viewType The type of the view
 * @param model The model used
 * @return The created controller
 */
Controller *initView(ViewType viewType, Model *model);
