#include "player.h"
#include "aliens.h"
#include "shield.h"
#include "game.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Player *createPlayer() {
  Player *res = (Player *)malloc(sizeof(Player));
  if (res == NULL) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }
  res->lives = DEFAULT_LIVES;
  res->position = 0.5;
  res->shootX = -1;
  res->shootY = -1;
  return res;
}

void freePlayer(Player *player) {
  if (player != NULL) {
    free(player);
  }
}

void playerMoveLeft(Player *player) {
  player->position -= PLAYER_SPEED;
  if (player->position < 0)
    player->position = 0;
}

void playerMoveRight(Player *player) {
  player->position += PLAYER_SPEED;
  if (player->position > 1)
    player->position = 1;
}

void playerFire(Player *player, Aliens *aliens) {
  if (player->shootX != -1 && player->shootY >= 0)
    return;
  player->shootX = player->position;
  player->shootY = PLAYER_SHOOT_START_Y;
  incrementShotCounter(aliens);
}

void updatePlayerShot(Player *player, Shields *shields) {
  if (player->shootX != -1 && player->shootY >= 0) {
    player->shootY -= SHOOT_SPEED;
    // Check collision with shields
    if (checkShieldCollision(shields, player->shootX, player->shootY)) {
      // Destroy the shot
      player->shootX = -1;
      player->shootY = -1;
    }
  }
}

int resolvePlayerHit(Player *player, Aliens *aliens) {
  const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;
  const double playerX = player->position * GAME_WIDTH_RATIO + margin;
  const double playerY = 1.0 - PLAYER_HEIGHT_RATIO / 2.0;

  const double halfHitW = PLAYER_HITBOX_WIDTH / 2.0;
  const double halfHitH = PLAYER_HITBOX_HEIGHT / 2.0;

  for (int i = 0; i < MAX_ALIEN_SHOTS; i++) {
    if (!aliens->alienShotActive[i])
      continue;

    const double shotX = aliens->alienShotX[i] * GAME_WIDTH_RATIO + margin;
    const double shotY = aliens->alienShotY[i];

    if (fabs(shotX - playerX) < halfHitW && fabs(shotY - playerY) < halfHitH) {
      aliens->alienShotActive[i] = 0;
      aliens->alienShotX[i] = -1;
      aliens->alienShotY[i] = -1;

      if (player->lives > 0) {
        player->lives--;
      }

      return 1;
    }
  }
  return 0;
}
