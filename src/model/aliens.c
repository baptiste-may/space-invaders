#include "aliens.h"
#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Aliens *createAliens(unsigned nbAliens, unsigned nbAlienRows) {
  Aliens *res = (Aliens *)malloc(sizeof(Aliens));
  if (res == NULL) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }
  
  res->nbAliens = nbAliens;
  res->nbAlienRows = nbAlienRows;
  res->aliensX = 0.5;
  res->aliensY = 0;
  res->alienMovement = ALIEN_SPEED_X;
  
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

void freeAliens(Aliens *aliens) {
  if (aliens != NULL) {
    free(aliens->aliens);
    free(aliens);
  }
}

int moveAliens(Aliens *aliens) {
    aliens->aliensX += aliens->alienMovement;
    if (aliens->aliensX < 0 || aliens->aliensX > 1) {
      aliens->alienMovement = -aliens->alienMovement;
      aliens->aliensX += aliens->alienMovement;
      aliens->aliensY += ALIEN_SPEED_Y;
      if (aliens->aliensY > 1) {
        aliens->aliensY -= ALIEN_SPEED_Y;
        aliens->alienMovement = 0;
        return 1; // Game Over
      }
    }
    return 0;
}

void alienShoot(Aliens *aliens) {
  const double gridHeight = ALIENS_HEIGHT_RATIO * ALIENS_GRID_HEIGHT_RATIO;
  const double moveRangeY =
      ALIENS_HEIGHT_RATIO * (1.0 - ALIENS_GRID_HEIGHT_RATIO);
  const double alienStepX = GAME_WIDTH_RATIO / aliens->nbAliens;
  const double alienBaseX = (GAME_WIDTH_RATIO * 0.5) / aliens->nbAliens +
                            aliens->aliensX *
                                (ALIENS_SWAY_FACTOR / aliens->nbAliens);
  const double rowHeight = gridHeight / aliens->nbAlienRows;
  const double alienBaseY = HEADER_HEIGHT_RATIO + aliens->aliensY * moveRangeY;

  for (unsigned i = 0; i < aliens->nbAlienRows; i++) {
    for (unsigned j = 0; j < aliens->nbAliens; j++) {
      unsigned k = j + i * aliens->nbAliens;
      
      if (aliens->aliens[k] < 0)
        continue;

      double randVal = (double)rand() / RAND_MAX;
      if (randVal < ALIEN_SHOOT_PROBABILITY) {
        for (int s = 0; s < MAX_ALIEN_SHOTS; s++) {
          if (!aliens->alienShotActive[s]) {
            double alienX = alienStepX * j + alienBaseX;
            double alienY = rowHeight * (i + 0.5) + alienBaseY;
            
            const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;
            aliens->alienShotX[s] = (alienX - margin) / GAME_WIDTH_RATIO;
            aliens->alienShotY[s] = alienY;
            aliens->alienShotActive[s] = 1;
            
            break;
          }
        }
      }
    }
  }
}

void updateAlienShots(Aliens *aliens) {
  for (int i = 0; i < MAX_ALIEN_SHOTS; i++) {
    if (aliens->alienShotActive[i]) {
      aliens->alienShotY[i] += ALIEN_SHOOT_SPEED;
      
      if (aliens->alienShotY[i] > 1.0) {
        aliens->alienShotActive[i] = 0;
        aliens->alienShotX[i] = -1;
        aliens->alienShotY[i] = -1;
      }
    }
  }
}

void animateAliens(Aliens *aliens) {
    for (unsigned i = 0; i < aliens->nbAlienRows; i++) {
      for (unsigned j = 0; j < aliens->nbAliens; j++) {
        unsigned k = j + i * aliens->nbAliens;
        if (aliens->aliens[k] < 0)
          continue;
        aliens->aliens[k] += aliens->aliens[k] % 2 ? -1 : 1;
      }
    }
}

int resolveAlienHit(Aliens *aliens, double shotX_norm, double shotY_norm) {
  if (shotX_norm == -1 || shotY_norm < HEADER_HEIGHT_RATIO)
    return 0;

  const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;
  const double shotX = shotX_norm * GAME_WIDTH_RATIO + margin;
  const double shotY = shotY_norm;

  const double gridHeight = ALIENS_HEIGHT_RATIO * ALIENS_GRID_HEIGHT_RATIO;
  const double moveRangeY =
      ALIENS_HEIGHT_RATIO * (1.0 - ALIENS_GRID_HEIGHT_RATIO);

  const double halfHitW =
      (ALIEN_HITBOX_WIDTH_RATIO * GAME_WIDTH_RATIO) / aliens->nbAliens / 2.0;
  const double halfHitH =
      (ALIEN_HITBOX_HEIGHT_RATIO * gridHeight) / aliens->nbAlienRows / 2.0;

  const double alienStepX = GAME_WIDTH_RATIO / aliens->nbAliens;
  const double alienBaseX = (GAME_WIDTH_RATIO * 0.5) / aliens->nbAliens +
                            aliens->aliensX *
                                (ALIENS_SWAY_FACTOR / aliens->nbAliens);

  const double rowHeight = gridHeight / aliens->nbAlienRows;
  const double alienBaseY =
      HEADER_HEIGHT_RATIO + aliens->aliensY * moveRangeY;

  for (unsigned i = 0; i < aliens->nbAlienRows; i++) {
    double alienY = rowHeight * (i + 0.5) + alienBaseY;
    if (fabs(shotY - alienY) >= halfHitH)
      continue;

    for (unsigned j = 0; j < aliens->nbAliens; j++) {
      unsigned k = j + i * aliens->nbAliens;
      if (aliens->aliens[k] < 0)
        continue;

      double alienX = alienStepX * j + alienBaseX;

      if (fabs(shotX - alienX) < halfHitW) {
        int alienType = aliens->aliens[k] / 2;
        aliens->aliens[k] = -1;
        return (alienType + 1) * 10;
      }
    }
  }
  return 0;
}
