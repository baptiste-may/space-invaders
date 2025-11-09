#ifndef MODEL_H
#define MODEL_H

#include "game.h"

/**
 * @struct Model
 * @brief Represent the model
 */
typedef struct {
  Game *currentGame; //!< The current game, NULL is there is not
} Model;

/**
 * @breif Start a game from a model
 *
 * @param model The model
 */
Game *startGame(Model *model);
/**
 * @breif Destroy the model by closing and freeing all things inside
 *
 * @param model The model
 */
void destroyModel(Model *model);

#endif
