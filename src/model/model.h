#pragma once

#include "game.h"
#include "menu.h"

/**
 * @struct Model
 * @brief Represent the model
 */
typedef struct {
  Game *currentGame; //!< The current game, NULL if none
  Menu mainMenu;     //!< The informations about the main menu
  Menu creditsMenu;  //!< The informations about the credits menu
  Menu gameOverMenu; //!< The informations about the game over menu
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
