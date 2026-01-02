#include "aliens.h"
#include "game.h"
#include "shield.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static const int UFO_POINTS[] = {100, 50,  50,  100, 150, 100, 100, 50,
                                 300, 100, 100, 100, 50,  150, 100, 50};

Aliens *createAliens(unsigned nbAliens, unsigned nbAlienRows,
                     double speedMultiplier) {
  Aliens *res = (Aliens *)malloc(sizeof(Aliens));
  if (res == NULL) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }

  res->nbAliens = nbAliens;
  res->nbAlienRows = nbAlienRows;
  res->aliensX = 0.5;
  res->aliensY = 0;
  res->alienMovement = ALIEN_SPEED_X * speedMultiplier;

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

  for (int i = 0; i < MAX_ALIEN_SHOOTS; i++) {
    res->alienShootStatus[i] = 0;
    res->alienShootX[i] = -1;
    res->alienShootY[i] = -1;
  }

  // UFO initialization
  res->ufoActive = false;
  res->ufoX = -1;
  res->ufoDirection = 1;
  res->shootCounter = 0;

  return res;
}

void freeAliens(Aliens *aliens) {
  if (aliens != NULL) {
    free(aliens->aliens);
    free(aliens);
  }
}

bool moveAliens(Aliens *aliens) {
  aliens->aliensX += aliens->alienMovement;
  if (aliens->aliensX < 0 || aliens->aliensX > 1) {
    aliens->alienMovement = -aliens->alienMovement;
    aliens->aliensX += aliens->alienMovement;
    aliens->aliensY += ALIEN_SPEED_Y;
    if (aliens->aliensY > 1) {
      aliens->aliensY -= ALIEN_SPEED_Y;
      aliens->alienMovement = 0;
      return true; // Game Over
    }
  }
  return false;
}

void alienShoot(Aliens *aliens) {
  const double gridHeight = ALIENS_HEIGHT_RATIO * ALIENS_GRID_HEIGHT_RATIO;
  const double moveRangeY =
      ALIENS_HEIGHT_RATIO * (1.0 - ALIENS_GRID_HEIGHT_RATIO);
  const double alienStepX = GAME_WIDTH_RATIO / aliens->nbAliens;
  const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;
  const double alienBaseX =
      margin + (GAME_WIDTH_RATIO * 0.5) / aliens->nbAliens +
      (aliens->aliensX - 0.5) * (ALIENS_SWAY_FACTOR / aliens->nbAliens);
  const double rowHeight = gridHeight / aliens->nbAlienRows;
  // FIX: Aliens start below the UFO (UFO_HEIGHT_RATIO + small gap)
  const double alienBaseY =
      UFO_HEIGHT_RATIO + 0.05 + aliens->aliensY * moveRangeY;

  for (unsigned i = 0; i < aliens->nbAlienRows; i++) {
    for (unsigned j = 0; j < aliens->nbAliens; j++) {
      unsigned k = j + i * aliens->nbAliens;

      if (aliens->aliens[k] < 0)
        continue;

      double randVal = (double)rand() / RAND_MAX;
      if (randVal < ALIEN_SHOOT_PROBABILITY) {
        for (int s = 0; s < MAX_ALIEN_SHOOTS; s++) {
          if (!aliens->alienShootStatus[s]) {
            double alienX = alienStepX * j + alienBaseX;
            double alienY = rowHeight * (i + 0.5) + alienBaseY;

            aliens->alienShootX[s] = (alienX - margin) / GAME_WIDTH_RATIO;
            aliens->alienShootY[s] = alienY;
            aliens->alienShootStatus[s] = (rand() % NB_SHOOT_SPRITE) + 1;

            break;
          }
        }
      }
    }
  }
}

void updateAlienShoots(Aliens *aliens, Shields *shields) {
  for (int i = 0; i < MAX_ALIEN_SHOOTS; i++) {
    if (aliens->alienShootStatus[i]) {
      aliens->alienShootY[i] += ALIEN_SHOOT_SPEED;

      // Check collision with shields
      if (checkShieldCollision(shields, aliens->alienShootX[i],
                               aliens->alienShootY[i])) {
        aliens->alienShootStatus[i] = 0;
        aliens->alienShootX[i] = -1;
        aliens->alienShootY[i] = -1;
        continue;
      }

      if (aliens->alienShootY[i] > 1.0) {
        aliens->alienShootStatus[i] = 0;
        aliens->alienShootX[i] = -1;
        aliens->alienShootY[i] = -1;
      }
    }
  }
}

void animateAliens(Aliens *aliens, bool isTick) {
  for (unsigned i = 0; i < aliens->nbAlienRows; i++) {
    for (unsigned j = 0; j < aliens->nbAliens; j++) {
      unsigned k = j + i * aliens->nbAliens;
      if (aliens->aliens[k] < 0) {
        if (aliens->aliens[k] >= -EXPLOSION_FRAMES)
          aliens->aliens[k]--;
        continue;
      }
      if (isTick)
        aliens->aliens[k] += aliens->aliens[k] % 2 ? -1 : 1;
    }
  }
}

void updateUFO(Aliens *aliens) {
  if (aliens->ufoActive) {
    aliens->ufoX += aliens->ufoDirection * UFO_SPEED;

    // UFO goes off screen
    if (aliens->ufoX < -0.1 || aliens->ufoX > 1.1) {
      aliens->ufoActive = false;
      aliens->ufoX = -1;
    }
  } else {
    // Spawn chance
    double randVal = (double)rand() / RAND_MAX;
    if (randVal < UFO_SPAWN_CHANCE) {
      aliens->ufoActive = true;
      // Random spawn from left or right
      aliens->ufoDirection = (rand() % 2) ? 1 : -1;
      aliens->ufoX = aliens->ufoDirection == 1 ? -0.05 : 1.05;
    }
  }
}

void incrementShootCounter(Aliens *aliens) {
  aliens->shootCounter = (aliens->shootCounter + 1) % 15;
}

int resolveUFOHit(Aliens *aliens, double shootX_norm, double shootY_norm) {
  if (!aliens->ufoActive || shootX_norm == -1)
    return 0;

  const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;
  const double shootX = shootX_norm * GAME_WIDTH_RATIO + margin;
  const double shootY = shootY_norm;

  const double ufoX = aliens->ufoX;
  const double ufoY = UFO_HEIGHT_RATIO;

  const double halfHitW = 0.04;
  const double halfHitH = 0.02;

  if (fabs(shootX - ufoX) < halfHitW && fabs(shootY - ufoY) < halfHitH) {
    aliens->ufoActive = false;
    aliens->ufoX = -1;
    return UFO_POINTS[aliens->shootCounter];
  }

  return 0;
}

int resolveAlienHit(Aliens *aliens, double shootX_norm, double shootY_norm) {
  if (shootX_norm == -1 || shootY_norm < UFO_HEIGHT_RATIO + 0.05)
    return 0;

  const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;
  const double shootX = shootX_norm * GAME_WIDTH_RATIO + margin;
  const double shootY = shootY_norm;

  const double gridHeight = ALIENS_HEIGHT_RATIO * ALIENS_GRID_HEIGHT_RATIO;
  const double moveRangeY =
      ALIENS_HEIGHT_RATIO * (1.0 - ALIENS_GRID_HEIGHT_RATIO);

  const double halfHitW =
      (ALIEN_HITBOX_WIDTH_RATIO * GAME_WIDTH_RATIO) / aliens->nbAliens / 2.0;
  const double halfHitH =
      (ALIEN_HITBOX_HEIGHT_RATIO * gridHeight) / aliens->nbAlienRows / 2.0;

  const double alienStepX = GAME_WIDTH_RATIO / aliens->nbAliens;
  const double alienBaseX =
      margin + (GAME_WIDTH_RATIO * 0.5) / aliens->nbAliens +
      (aliens->aliensX - 0.5) * (ALIENS_SWAY_FACTOR / aliens->nbAliens);

  const double rowHeight = gridHeight / aliens->nbAlienRows;
  // Ensure aliens start below the UFO so they don't overlap with the score area
  // or the UFO path.
  const double alienBaseY =
      UFO_HEIGHT_RATIO + 0.05 + aliens->aliensY * moveRangeY;

  for (unsigned i = 0; i < aliens->nbAlienRows; i++) {
    double alienY = rowHeight * (i + 0.5) + alienBaseY;
    if (fabs(shootY - alienY) >= halfHitH)
      continue;

    for (unsigned j = 0; j < aliens->nbAliens; j++) {
      unsigned k = j + i * aliens->nbAliens;
      if (aliens->aliens[k] < 0)
        continue;

      double alienX = alienStepX * j + alienBaseX;

      if (fabs(shootX - alienX) < halfHitW) {
        int alienType = aliens->aliens[k] / 2;
        aliens->aliens[k] = -1;
        return (alienType + 1) * 10;
      }
    }
  }
  return 0;
}
