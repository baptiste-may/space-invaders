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
                -1,
                {0},
                {0},
                {0},
                0};

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

  for (int i = 0; i < MAX_ALIEN_SHOTS; i++) {
    res->alienShotActive[i] = 0;
    res->alienShotX[i] = -1;
    res->alienShotY[i] = -1;
  }

  return res;
}

void freeGame(Game *game) {
  free(game->aliens);
  game->aliens = NULL;
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
      (ALIEN_HITBOX_WIDTH_RATIO * GAME_WIDTH_RATIO) / game->nbAliens / 2.0;
  const double halfHitH =
      (ALIEN_HITBOX_HEIGHT_RATIO * gridHeight) / game->nbAlienRows / 2.0;

  const double alienStepX = GAME_WIDTH_RATIO / game->nbAliens;
  const double alienBaseX = (GAME_WIDTH_RATIO * 0.5) / game->nbAliens +
                            game->aliensX *
                                (ALIENS_SWAY_FACTOR / game->nbAliens);

  const double rowHeight = gridHeight / game->nbAlienRows;
  const double alienBaseY =
      HEADER_HEIGHT_RATIO + game->aliensY * moveRangeY;

  for (unsigned i = 0; i < game->nbAlienRows; i++) {
    double alienY = rowHeight * (i + 0.5) + alienBaseY;
    if (fabs(shotY - alienY) >= halfHitH)
      continue;

    for (unsigned j = 0; j < game->nbAliens; j++) {
      unsigned k = j + i * game->nbAliens;
      if (game->aliens[k] < 0)
        continue;

      double alienX = alienStepX * j + alienBaseX;

      if (fabs(shotX - alienX) < halfHitW) {
        int alienType = game->aliens[k] / 2;
        game->aliens[k] = -1;
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
    if (!game->alienShotActive[i])
      continue;

    const double shotX = game->alienShotX[i] * GAME_WIDTH_RATIO + margin;
    const double shotY = game->alienShotY[i];

    if (fabs(shotX - playerX) < halfHitW && fabs(shotY - playerY) < halfHitH) {
      game->alienShotActive[i] = 0;
      game->alienShotX[i] = -1;
      game->alienShotY[i] = -1;
      
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

void alienShoot(Game *game) {
  const double gridHeight = ALIENS_HEIGHT_RATIO * ALIENS_GRID_HEIGHT_RATIO;
  const double moveRangeY =
      ALIENS_HEIGHT_RATIO * (1.0 - ALIENS_GRID_HEIGHT_RATIO);
  const double alienStepX = GAME_WIDTH_RATIO / game->nbAliens;
  const double alienBaseX = (GAME_WIDTH_RATIO * 0.5) / game->nbAliens +
                            game->aliensX *
                                (ALIENS_SWAY_FACTOR / game->nbAliens);
  const double rowHeight = gridHeight / game->nbAlienRows;
  const double alienBaseY = HEADER_HEIGHT_RATIO + game->aliensY * moveRangeY;

  for (unsigned i = 0; i < game->nbAlienRows; i++) {
    for (unsigned j = 0; j < game->nbAliens; j++) {
      unsigned k = j + i * game->nbAliens;
      
      if (game->aliens[k] < 0)
        continue;

      double randVal = (double)rand() / RAND_MAX;
      if (randVal < ALIEN_SHOOT_PROBABILITY) {
        for (int s = 0; s < MAX_ALIEN_SHOTS; s++) {
          if (!game->alienShotActive[s]) {
            double alienX = alienStepX * j + alienBaseX;
            double alienY = rowHeight * (i + 0.5) + alienBaseY;
            
            const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;
            game->alienShotX[s] = (alienX - margin) / GAME_WIDTH_RATIO;
            game->alienShotY[s] = alienY;
            game->alienShotActive[s] = 1;
            
            break;
          }
        }
      }
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
  for (int i = 0; i < MAX_ALIEN_SHOTS; i++) {
    if (game->alienShotActive[i]) {
      game->alienShotY[i] += ALIEN_SHOOT_SPEED;
      
      if (game->alienShotY[i] > 1.0) {
        game->alienShotActive[i] = 0;
        game->alienShotX[i] = -1;
        game->alienShotY[i] = -1;
      }
    }
  }

  checkPlayerHit(game);

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
      if (game->aliensY > 1) {
        game->aliensY -= ALIEN_SPEED_Y;
        game->alienMovement = 0;
        game->gameOver = 1;
      }
    }

    alienShoot(game);
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