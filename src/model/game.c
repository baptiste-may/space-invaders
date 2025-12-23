#include "game.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Game *newGame(unsigned int nbAliens, unsigned int nbAlienRows,
              unsigned int nbBuildings) {
  srand(time(NULL));
  
  // Creating game
  Game *res = (Game *)malloc(sizeof(Game));
  if (res == NULL) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }
  
  Aliens *aliens = createAliens(nbAliens, nbAlienRows);

  *res = (Game){0,
                DEFAULT_LIVES,
                nbBuildings,
                0,
                DEFAULT_FRAME_LENGTH,
                aliens,
                0.5,
                -1,
                -1,
                0};

  return res;
}

void freeGame(Game *game) {
  freeAliens(game->aliens);
  free(game);
}

void checkCollisions(Game *game) {
  if (game->playerShootX == -1 || game->playerShootY < HEADER_HEIGHT_RATIO)
    return;

  const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;
  const double shotX = game->playerShootX * GAME_WIDTH_RATIO + margin;
  const double shotY = game->playerShootY;

  const double gridHeight = ALIENS_HEIGHT_RATIO * ALIENS_GRID_HEIGHT_RATIO;
  const double moveRangeY =
      ALIENS_HEIGHT_RATIO * (1.0 - ALIENS_GRID_HEIGHT_RATIO);

  const double halfHitW =
      (ALIEN_HITBOX_WIDTH_RATIO * GAME_WIDTH_RATIO) / game->aliens->nbAliens / 2.0;
  const double halfHitH =
      (ALIEN_HITBOX_HEIGHT_RATIO * gridHeight) / game->aliens->nbAlienRows / 2.0;

  const double alienStepX = GAME_WIDTH_RATIO / game->aliens->nbAliens;
  const double alienBaseX = (GAME_WIDTH_RATIO * 0.5) / game->aliens->nbAliens +
                            game->aliens->aliensX *
                                (ALIENS_SWAY_FACTOR / game->aliens->nbAliens);

  const double rowHeight = gridHeight / game->aliens->nbAlienRows;
  const double alienBaseY =
      HEADER_HEIGHT_RATIO + game->aliens->aliensY * moveRangeY;

  for (unsigned i = 0; i < game->aliens->nbAlienRows; i++) {
    double alienY = rowHeight * (i + 0.5) + alienBaseY;
    if (fabs(shotY - alienY) >= halfHitH)
      continue;

    for (unsigned j = 0; j < game->aliens->nbAliens; j++) {
      unsigned k = j + i * game->aliens->nbAliens;
      if (game->aliens->aliens[k] < 0)
        continue;

      double alienX = alienStepX * j + alienBaseX;

      if (fabs(shotX - alienX) < halfHitW) {
        int alienType = game->aliens->aliens[k] / 2;
        game->aliens->aliens[k] = -1;
        game->playerShootX = -1;
        game->playerShootY = -1;
        game->score += (alienType + 1) * 10;
        return;
      }
    }
  }
}

void checkPlayerHit(Game *game) {
  if (game->gameOver)
    return;
    
  const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;
  const double playerX = game->playerPosition * GAME_WIDTH_RATIO + margin;
  const double playerY = 1.0 - PLAYER_HEIGHT_RATIO / 2.0;
  
  const double halfHitW = PLAYER_HITBOX_WIDTH / 2.0;
  const double halfHitH = PLAYER_HITBOX_HEIGHT / 2.0;

  for (int i = 0; i < MAX_ALIEN_SHOTS; i++) {
    if (!game->aliens->alienShotActive[i])
      continue;

    const double shotX = game->aliens->alienShotX[i] * GAME_WIDTH_RATIO + margin;
    const double shotY = game->aliens->alienShotY[i];

    if (fabs(shotX - playerX) < halfHitW && fabs(shotY - playerY) < halfHitH) {
      game->aliens->alienShotActive[i] = 0;
      game->aliens->alienShotX[i] = -1;
      game->aliens->alienShotY[i] = -1;
      
      if (game->lives > 0) {
        game->lives--;
      }
      
      if (game->lives == 0) {
        game->gameOver = 1;
        exit(EXIT_SUCCESS);
      }
      
      return;
    }
  }
}

void nextFrame(Game *game) {
  game->frame = (game->frame + 1) % game->frameMax;

  // Player shoot
  if (game->playerShootX != -1 && game->playerShootY >= 0) {
    game->playerShootY -= SHOOT_SPEED;

    // Vérifier les collisions à chaque frame
    checkCollisions(game);

    if (game->playerShootY <= HEADER_HEIGHT_RATIO) {
      game->playerShootY = -1;
      game->playerShootX = -1;
    }
  }

  // Alien shots
  updateAlienShots(game->aliens);

  checkPlayerHit(game);

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
  game->playerShootY = PLAYER_SHOOT_START_Y;
}
