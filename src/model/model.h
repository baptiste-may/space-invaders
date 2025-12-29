#pragma once

#include "game.h"
#include "main-menu.h"

/**
 * @struct Model
 * @brief Represent the model
 */
typedef struct {
  Game *currentGame; //!< The current game, NULL if none
  MainMenu mainMenu; //!< The informations about the main menu
  int gameOverSelected; //!< The information about the game over option selected
} Model;

/**
 * @brief Create a new model
 *
 * @return The model created
 */
Model newModel();

/**
 * @brief Start a new game instance
 *
 * @param model The model
 */
void *startGame(Model *model);
/**
 * @brief Free the model and its resources
 *
 * @param model The model
 */
void destroyModel(Model *model);
