#include "game.h"
#include "shield.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Game *newGame(unsigned int nbAliens, unsigned int nbAlienRows,
              unsigned int nbShields) {
  srand(time(NULL));

  // Creating game
  Game *res = (Game *)malloc(sizeof(Game));
  if (res == NULL) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }

  Aliens *aliens = createAliens(nbAliens, nbAlienRows);
  Player *player = createPlayer();
  Shield *shields = createShields(nbShields);

  *res = (Game){0,      0,      DEFAULT_FRAME_LENGTH,
                aliens, player, (Shields){nbShields, shields},
                0};

  return res;
}

void freeGame(Game *game) {
  freeAliens(game->aliens);
  freePlayer(game->player);
  freeShields(&(game->shields));
  free(game);
}

void nextFrame(Game *game) {
  game->frame = (game->frame + 1) % game->frameMax;

  // UFO update
  updateUFO(game->aliens);

  // Player shoot
  if (game->player->shootX != -1 && game->player->shootY >= 0) {
    updatePlayerShot(game->player, &game->shields);

    // Check collision with UFO
    int ufoPoints =
        resolveUFOHit(game->aliens, game->player->shootX, game->player->shootY);
    if (ufoPoints > 0) {
      game->score += ufoPoints;
      game->player->shootX = -1;
      game->player->shootY = -1;
    }

    // Check collisions every frame
    if (game->player->shootX != -1) {
      int points = resolveAlienHit(game->aliens, game->player->shootX,
                                   game->player->shootY);
      if (points > 0) {
        game->score += points;
        game->player->shootX = -1;
        game->player->shootY = -1;
      }
    }
    if (game->player->shootY <= HEADER_HEIGHT_RATIO) {
      game->player->shootY = -1;
      game->player->shootX = -1;
    }
  }

  // Alien shots
  updateAlienShots(game->aliens, &game->shields);

  // Check player hit
  if (!game->gameOver) {
    if (resolvePlayerHit(game->player, game->aliens)) {
      if (game->player->lives == 0) {
        game->gameOver = 1;
        exit(EXIT_SUCCESS);
      }
    }
  }

  // Next tick
  if (game->frame == 0) {
    // Aliens sprites
    animateAliens(game->aliens);

    // Aliens positions
    if (moveAliens(game->aliens)) {
      game->gameOver = 1;
    }

    alienShoot(game->aliens);
  }
}
