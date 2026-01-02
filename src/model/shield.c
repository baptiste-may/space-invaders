#include "shield.h"
#include "game.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const ShieldBlock shieldTemplate[SHIELD_HEIGHT][SHIELD_WIDTH] = {
    {EMPTY, HALF_EMPTY_RIGHT, FULL, FULL, FULL, FULL, FULL, FULL, FULL,
     HALF_EMPTY_LEFT, EMPTY},
    {HALF_EMPTY_RIGHT, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL,
     HALF_EMPTY_LEFT},
    {FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL},
    {FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL},
    {FULL, FULL, HALF_FULL_RIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
     HALF_FULL_LEFT, FULL, FULL}};

Shield *createShields(unsigned nbShields) {
  Shield *res = (Shield *)malloc(sizeof(Shield) * nbShields);
  if (!res) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }

  for (unsigned n = 0; n < nbShields; n++) {
    for (unsigned i = 0; i < SHIELD_HEIGHT; i++) {
      for (unsigned j = 0; j < SHIELD_WIDTH; j++) {
        res[n][i][j] = shieldTemplate[i][j];
      }
    }
  }

  return res;
}

void freeShields(Shields *shields) {
  if (shields && shields->blocks)
    free(shields->blocks);
}

/**
 * @brief Apply damage to a single shield block
 *
 * @param block The block to damage
 */
static void damageBlock(ShieldBlock *block) {
  switch (*block) {
  case EMPTY:
    break;
  case FULL:
    *block = (rand() % 2) ? HALF_EMPTY_LEFT : HALF_EMPTY_RIGHT;
    break;
  case HALF_FULL_LEFT:
    *block = EMPTY;
    break;
  case HALF_FULL_RIGHT:
    *block = EMPTY;
    break;
  case HALF_EMPTY_LEFT:
    *block = EMPTY;
    break;
  case HALF_EMPTY_RIGHT:
    *block = EMPTY;
    break;
  case DESTROYED_LEFT:
  case DESTROYED_RIGHT:
    *block = EMPTY;
    break;
  }
}

void damageShield(Shield shield, int x, int y) {
  if (x < 0 || x >= SHIELD_WIDTH || y < 0 || y >= SHIELD_HEIGHT) {
    return;
  }

  damageBlock(&shield[y][x]);

  if (y > 0) {
    damageBlock(&shield[y - 1][x]);
  }

  if (y < SHIELD_HEIGHT - 1) {
    damageBlock(&shield[y + 1][x]);
  }

  if (x > 0) {
    damageBlock(&shield[y][x - 1]);
  }

  if (x < SHIELD_WIDTH - 1) {
    damageBlock(&shield[y][x + 1]);
  }
}

bool checkShieldCollision(Shields *shields, double shootX, double shootY) {
  // shootX is in GAME_WIDTH_RATIO space (0.0 to 1.0 in the game area)
  // It needs to be converted to full screen space (0.0 to 1.0)
  const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;
  const double shootXScreen = shootX * GAME_WIDTH_RATIO + margin;

  // Y position of shields
  const double shieldsY = 1.0 - PLAYER_HEIGHT_RATIO - SHIELD_HEIGHT_RATIO / 2.0;

  // Width of each section (in full screen space)
  const double shieldSectionWidth = 1.0 / shields->nb;

  // Approximation of shield size
  const double shieldWidthRatio = shieldSectionWidth * 0.2;
  const double shieldHeightRatio = SHIELD_HEIGHT_RATIO;

  const double halfHitH = shieldHeightRatio / 2.0;
  const double halfHitW = shieldWidthRatio / 2.0;

  // Check height
  if (fabs(shootY - shieldsY) >= halfHitH) {
    return false;
  }

  // For each shield
  for (unsigned n = 0; n < shields->nb; n++) {
    // Center of shield in full screen space
    const double shieldCenterX = shieldSectionWidth * (n + 0.5);

    // Check if in X zone
    if (fabs(shootXScreen - shieldCenterX) < halfHitW) {

      // Convert to block coordinates
      double relativeX =
          (shootXScreen - (shieldCenterX - halfHitW)) / shieldWidthRatio;
      double relativeY = (shootY - (shieldsY - halfHitH)) / shieldHeightRatio;

      int blockX = (int)(relativeX * SHIELD_WIDTH);
      int blockY = (int)(relativeY * SHIELD_HEIGHT);

      if (blockX >= 0 && blockX < SHIELD_WIDTH && blockY >= 0 &&
          blockY < SHIELD_HEIGHT) {
        if (shields->blocks[n][blockY][blockX] != EMPTY) {
          damageShield(shields->blocks[n], blockX, blockY);
          return true;
        }
      }
    }
  }

  return false;
}
