#include "game.h"
#include "aliens.h"
#include "score.h"
#include "shield.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Game *newGame(unsigned int nbAliens, unsigned int nbAlienRows,
              unsigned int nbShields) {
  srand(time(NULL));

  // Create game
  Game *res = (Game *)malloc(sizeof(Game));
  if (res == NULL) {
    perror("Allocation error");
    exit(EXIT_FAILURE);
  }

  Aliens *aliens = createAliens(nbAliens, nbAlienRows, 1.0);
  Player *player = createPlayer();
  Shield *shields = createShields(nbShields);

  *res = (Game){0,
                DEFAULT_FRAME_LENGTH,
                (Scores){0, getBestScore()},
                aliens,
                player,
                (Shields){nbShields, shields},
                false,
                -1,   // playerDeathFrame
                0,    // waveTransitionFrame
                1.0}; // currentAlienSpeed

  return res;
}

void freeGame(Game *game) {
  freeAliens(game->aliens);
  freePlayer(game->player);
  freeShields(&(game->shields));
  free(game);
}

void nextFrame(Game *game) {
  // Handle player death animation - freeze everything
  if (game->playerDeathFrame >= 0) {
    game->playerDeathFrame++;
    if (game->playerDeathFrame >= PLAYER_DEATH_FRAMES) {
      game->playerDeathFrame = -1;
      // Player stays at same position
    }
    // Don't increment frame counter, completely freeze the game
    return;
  }

  game->frame = (game->frame + 1) % game->frameMax;

  // Handle wave transition (fast restart)
  if (game->waveTransitionFrame > 0) {
    game->waveTransitionFrame++;
    if (game->waveTransitionFrame >= 30) { // 30 frames = 0.5 seconds
      // Increase speed for next wave (20% faster)
      game->currentAlienSpeed *= 1.2;

      // Recreate aliens at the top with new speed
      freeAliens(game->aliens);
      game->aliens = createAliens(11, 5, game->currentAlienSpeed);

      // Give bonus life to player
      game->player->lives++;

      game->waveTransitionFrame = 0;
    }
    return;
  }

  // Check if all aliens are dead
  bool allDead = true;
  for (unsigned i = 0; i < game->aliens->nbAlienRows * game->aliens->nbAliens;
       i++) {
    if (game->aliens->aliens[i] >= -EXPLOSION_FRAMES) {
      allDead = false;
      break;
    }
  }

  if (allDead) {
    game->waveTransitionFrame = 1;
    return;
  }

  // UFO update
  updateUFO(game->aliens);

  // Handle player shooting
  if (game->player->shootX != -1 && game->player->shootY >= 0) {
    updatePlayerShot(game->player, &game->shields);

    // Check collision with UFO
    int ufoPoints =
        resolveUFOHit(game->aliens, game->player->shootX, game->player->shootY);
    if (ufoPoints > 0) {
      game->scores.current += ufoPoints;
      game->player->shootX = -1;
      game->player->shootY = -1;
    }

    // Check collisions every frame
    if (game->player->shootX != -1) {
      int points = resolveAlienHit(game->aliens, game->player->shootX,
                                   game->player->shootY);
      if (points > 0) {
        game->scores.current += points;
        game->player->shootX = -1;
        game->player->shootY = -1;
      }
    }
    if (game->player->shootY <= HEADER_HEIGHT_RATIO) {
      game->player->shootY = -1;
      game->player->shootX = -1;
    }
  }

  // Update best score
  updateBestScore(&(game->scores));

  // Update alien animations
  animateAliens(game->aliens, game->frame == 0);

  // Update alien shots
  updateAlienShots(game->aliens, &game->shields);

  // Check if player is hit
  if (!game->gameOver) {
    if (resolvePlayerHit(game->player, game->aliens)) {
      if (game->player->lives == 0) {
        game->gameOver = true;
      } else {
        // Start death animation
        game->playerDeathFrame = 0;
      }
    }
  }

  // Handle game tick
  if (game->frame == 0) {
    // Aliens positions
    if (moveAliens(game->aliens)) {
      game->gameOver = true;
    }

    alienShoot(game->aliens);
  }
}
