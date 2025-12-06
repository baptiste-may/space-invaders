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

void checkCollisions(Game *game) {
  if (game->playerShootX == -1 || game->playerShootY < 0)
    return;

  const double ALIEN_HITBOX_X = 0.015;  // Largeur de collision
  const double ALIEN_HITBOX_Y = 0.020;  // Hauteur de collision
  
  const double marginX = 0.078;
  const double startX = 0.039;
  
  for (unsigned i = 0; i < game->nbAlienRows; i++) {
    for (unsigned j = 0; j < game->nbAliens; j++) {
      unsigned k = j + i * game->nbAliens;
      
      // Si l'alien est déjà mort, on passe
      if (game->aliens[k] < 0)
        continue;
      
      double alienX = ((1.0 - marginX) / (double)(game->nbAliens)) * j + startX + game->aliensX * marginX;
      const double startY = 0.139;
      const double spacingY = 0.069;
      const double moveY = 0.208;
      double alienY = startY + spacingY * i + game->aliensY * moveY;
      
      // Distance entre le tir et l'alien
      double distX = fabs(game->playerShootX - alienX);
      double distY = fabs(game->playerShootY - alienY);
      
      // Vérifier la collision
      if (distX < ALIEN_HITBOX_X && distY < ALIEN_HITBOX_Y) {
        // L'alien est touché !
        int alienType = game->aliens[k] / 2;
        game->aliens[k] = -1; // Il est dead !
        
        // Supprimer le tir
        game->playerShootX = -1;
        game->playerShootY = -1;
        
        // Ajouter des points selon le type d'alien
        switch (alienType) {
          case 0: game->score += 10; break; // Alien Poulpe 
          case 1: game->score += 20; break; // Alien Crabe
          case 2: game->score += 30; break; // Alien Pieuvre
          default: game->score += 40; break; // Alien Rouge (faire la règle du mod(15))
        }
        return; // Un tir ne peut toucher qu'un seul alien
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
    
    if (game->playerShootY <= 0) {
      game->playerShootY = -1;
      game->playerShootX = -1;
    }
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