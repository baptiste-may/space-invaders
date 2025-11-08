#ifndef MODEL_H
#define MODEL_H

#include "game.h"

typedef struct {
  Game *currentGame;
} Model;

Game *startGame(Model *model);
void destroyModel(Model *model);

#endif
