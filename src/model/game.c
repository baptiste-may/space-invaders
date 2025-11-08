#include "game.h"
#include <stdio.h>
#include <stdlib.h>

Game *createGame(unsigned int nbAliens, unsigned int nbAlienRows,
                 unsigned int nbBuildings) {
  // Creating game
  Game *res = (Game *)malloc(sizeof(Game));
  if (res == NULL) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }
  *res = (Game){0, DEFAULT_LIVES, nbAliens, nbAlienRows, nbBuildings, NULL};

  // Building aliens
  res->aliens = (char *)calloc(sizeof(char), nbAlienRows * nbAliens);
  if (res->aliens == NULL) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }

  for (unsigned i = 0; i < nbAlienRows; i++) {
    for (unsigned j = 0; j < nbAliens; j++) {
      res->aliens[i * nbAliens + j] = nbAlienRows - i;
    }
  }

  return res;
}

void freeGame(Game *game) {
  free(game->aliens);
  game->aliens = NULL;
  free(game);
}
