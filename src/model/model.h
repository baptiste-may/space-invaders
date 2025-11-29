#pragma once

#include "game.h"
#include "main-menu.h"

/**
 * @struct Model
 * @brief Represent the model
 */
typedef struct {
  Game *currentGame; //!< The current game, NULL is there is not
  MainMenu mainMenu; //!< The informations about the main menu
} Model;

/**
 * @brief Create a new model
 *
 * @return The model created
 */
Model newModel();

/**
 * @brief Start a game from a model
 *
 * @param model The model
 */
void *startGame(Model *model);
/**
 * @brief Destroy the model by closing and freeing all things inside
 *
 * @param model The model
 */
void destroyModel(Model *model);
