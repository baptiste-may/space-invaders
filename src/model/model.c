#include "model.h"
#include "game.h"
#include <stdlib.h>

Game *startGame(Model *model) {
  model->currentGame = createGame(5, 11, 4);
  return model->currentGame;
}

void destroyModel(Model *model) {
  if (model->currentGame != NULL)
    freeGame(model->currentGame);
}
