#include "model.h"
#include "game.h"
#include "main-menu.h"
#include <stdlib.h>

Model newModel() { return (Model){NULL, (MainMenu){false, 0}}; }

void *startGame(Model *model) {
  model->currentGame = newGame(5, 11, 4);
  return model->currentGame;
}

void destroyModel(Model *model) {
  if (model->currentGame != NULL)
    freeGame(model->currentGame);
}
