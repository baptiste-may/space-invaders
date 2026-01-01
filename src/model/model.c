#include "model.h"
#include "game.h"
#include "menu.h"
#include <stdlib.h>

Model newModel() { return (Model){NULL, createMenu(3), createMenu(2)}; }

void *startGame(Model *model) {
  model->currentGame = newGame(11, 5, 4);
  return model->currentGame;
}

void destroyModel(Model *model) {
  if (model->currentGame != NULL)
    freeGame(model->currentGame);
}
