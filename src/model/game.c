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

void checkCollisions(Game *game, int screenWidth, int screenHeight) {
  if (game->playerShootX == -1 || game->playerShootY < 0)
    return;

  // Calculer le scale
  double scale = (double)(screenWidth) / 640 + (double)(screenHeight) / 480;

  // Convertir la position du tir en pixels
  double playerSizeX = 13 * scale;
  double shootPixelX = game->playerShootX * (screenWidth - playerSizeX * 2) + playerSizeX;
  double shootPixelY = game->playerShootY * (screenHeight - (20 + 8 * scale) * 2) + 20 + 8 * scale / 2.0;
  
  // Hitbox en pixels responsive
  const double ALIEN_HITBOX_X = 10 * scale;  // Largeur
  const double ALIEN_HITBOX_Y = 8 * scale;   // Hauteur
  
  for (unsigned i = 0; i < game->nbAlienRows; i++) {
    for (unsigned j = 0; j < game->nbAliens; j++) {
      unsigned k = j + i * game->nbAliens;
      
      // Verif si alien mort
      if (game->aliens[k] < 0)
        continue;
      
      // Calcul position exacte de l'alien en pixels
      double alienPixelX = ((screenWidth - 100) / (double)(game->nbAliens)) * j + 50 + game->aliensX * 100;
      double alienPixelY = 100 + 50 * i + game->aliensY * 150;
      
      // Distance en pixels entre le tir et l'alien
      double distX = fabs(shootPixelX - alienPixelX);
      double distY = fabs(shootPixelY - alienPixelY);
      
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
          case 0: game->score += 10; break;  // Alien Poulpe
          case 1: game->score += 20; break;  // Alien Crabe
          case 2: game->score += 40; break;  // Alien Pieuvre
          default: game->score += 100; break; // Alien Rouge (faire la règle du mod(15))
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
    
    // Vérifier les collisions à chaque frame avec les dimensions de l'écran
    // On utilise 1280x720 comme référence (WIDTH et HEIGHT de sdl-view.h)
    checkCollisions(game, 1280, 720);
    
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