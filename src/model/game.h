#pragma once

#include "aliens.h"

/**
 * @def DEFAULT_FRAME_LENGTH
 * @brief The number of frame before calculating the next tick
 */
#define DEFAULT_FRAME_LENGTH 60

/**
 * @def DEFAULT_LIVES
 * @brief The number of lives when starting a game
 */
#define DEFAULT_LIVES 3

/**
 * @def PLAYER_SPEED
 * @brief The player speed
 */
#define PLAYER_SPEED 0.005

/**
 * @def SHOOT_SPEED
 * @brief The player shoot speed
 */
#define SHOOT_SPEED 0.01

/**
 * @def HEADER_HEIGHT_RATIO
 * @brief The ratio of the screen height taken by the header (score and lives)
 */
#define HEADER_HEIGHT_RATIO 0.1

/**
 * @def PLAYER_HEIGHT_RATIO
 * @brief The ratio of the screen height taken by the player
 */
#define PLAYER_HEIGHT_RATIO 0.1

/**
 * @def SHIELD_HEIGHT_RATIO
 * @brief The ratio of the screen height taken by the shields
 */
#define SHIELD_HEIGHT_RATIO 0.15

/**
 * @def GAME_WIDTH_RATIO
 * @brief The ratio of the screen width taken by the game
 */
#define GAME_WIDTH_RATIO 0.9

/**
 * @def PLAYER_SHOOT_START_Y
 * @brief The initial Y position of the player's shoot
 */
#define PLAYER_SHOOT_START_Y 0.925

/**
 * @def PLAYER_HITBOX_WIDTH
 * @brief The width of the player hitbox
 */
#define PLAYER_HITBOX_WIDTH 0.04

/**
 * @def PLAYER_HITBOX_HEIGHT
 * @brief The height of the player hitbox
 */
#define PLAYER_HITBOX_HEIGHT 0.06

/**
 * @struct Game
 * @brief The structure of a game, including all the informations
 */
typedef struct {
  unsigned score;       //!< The current score
  unsigned lives;       //!< The number of lives from the player
  unsigned nbBuildings; //!< The number of buildings between the aliens and the
                        //!< player
  
  unsigned frame;    //!< The current frame of the game (for timing, from 0 to
                     //!< frameMax)
  unsigned frameMax; //!< The max number of frame

  Aliens *aliens;    //!< The aliens structure
  
  double playerPosition; //!< The player position in pourcentage
  double playerShootX;   //!< The x coords of the player shoot
  double playerShootY;   //!< The y coords of the player shoot
  
  int gameOver; //!< Game over flag (0 = playing, 1 = game over)
} Game;

/**
 * @brief Create game structure
 *
 * @param nbAliens The number of aliens for each row
 * @param nbAlienRows The number of alien rows
 * @param nbBuildings The number of buildings between the aliens and the player
 *
 * @return The game structure
 */
Game *newGame(unsigned nbAliens, unsigned nbAlienRows, unsigned nbBuildings);

/**
 * @brief Free game structure
 *
 * @param game The game structure
 */
void freeGame(Game *game);

/**
 * @brief Calculate next frame
 *
 * @param game The game structure
 */
void nextFrame(Game *game);

/**
 * @brief Move the player to the left
 *
 * @param game The game structure
 */
void movePlayerLeft(Game *game);

/**
 * @brief Move the player to the right
 *
 * @param game The game structure
 */
void movePlayerRight(Game *game);

/**
 * @brief Shoot as player is possible
 *
 * @param game The game structure
 */
void playerShoot(Game *game);