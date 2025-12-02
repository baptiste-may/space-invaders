#include "game.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Game *newGame(unsigned int nbAliens, unsigned int nbAlienRows,
              unsigned int nbBuildings) {
  // Creating game
  Game *res = (Game *)malloc(sizeof(Game));
  if (res == NULL) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }
  *res = (Game){0,
                DEFAULT_LIVES,
                nbAliens,
                nbAlienRows,
                nbBuildings,
                0,
                DEFAULT_FRAME_LENGTH,
                NULL,
                0.5,
                0,
                ALIEN_SPEED_X,
                0.5,
                -1,
                -1};

  // Building aliens
  res->aliens = (int *)calloc(sizeof(int), nbAlienRows * nbAliens);
  if (res->aliens == NULL) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }

  for (unsigned i = 0; i < nbAlienRows; i++) {
    for (unsigned j = 0; j < nbAliens; j++) {
      res->aliens[i * nbAliens + j] = i == 0 ? 5 : i <= 2 ? 3 : 1;
    }
  }

  return res;
}

void freeGame(Game *game) {
  free(game->aliens);
  game->aliens = NULL;
  free(game);
}

void nextFrame(Game *game) {
  game->frame = (game->frame + 1) % game->frameMax;

  // Player shoot
  if (game->playerShootX != -1 && game->playerShootY >= 0) {
    game->playerShootY -= SHOOT_SPEED;
    if (game->playerShootY <= 0)
      game->playerShootY = -1;
  }

  // Next tick
  if (game->frame == 0) {
    // Aliens sprites
    for (unsigned i = 0; i < game->nbAlienRows; i++) {
      for (unsigned j = 0; j < game->nbAliens; j++) {
        unsigned k = j + i * game->nbAliens;
        if (game->aliens[k] < 0)
          continue;
        game->aliens[k] += game->aliens[k] % 2 ? -1 : 1;
      }
    }

    // Aliens positions
    game->aliensX += game->alienMovement;
    if (game->aliensX < 0 || game->aliensX > 1) {
      game->alienMovement = -game->alienMovement;
      game->aliensX += game->alienMovement;
      game->aliensY += ALIEN_SPEED_Y;
    }
  }
}

void movePlayerLeft(Game *game) {
  game->playerPosition -= PLAYER_SPEED;
  if (game->playerPosition < 0)
    game->playerPosition = 0;
}

void movePlayerRight(Game *game) {
  game->playerPosition += PLAYER_SPEED;
  if (game->playerPosition > 1)
    game->playerPosition = 1;
}

void playerShoot(Game *game) {
  if (game->playerShootX != -1 && game->playerShootY >= 0)
    return;
  game->playerShootX = game->playerPosition;
  game->playerShootY = 1;
}
