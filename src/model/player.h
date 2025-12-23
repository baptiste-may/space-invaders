#pragma once

#include "aliens.h"

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
 * @def PLAYER_HEIGHT_RATIO
 * @brief The ratio of the screen height taken by the player
 */
#define PLAYER_HEIGHT_RATIO 0.1

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
 * @struct Player
 * @brief The structure representing the player
 */
typedef struct {
  unsigned lives;       //!< The number of lives from the player
  double position;      //!< The player position in percentage
  double shootX;        //!< The x coords of the player shoot
  double shootY;        //!< The y coords of the player shoot
} Player;

struct Aliens;

/**
 * @brief Create player structure
 *
 * @return The player structure
 */
Player *createPlayer();

/**
 * @brief Free player structure
 *
 * @param player The player structure
 */
void freePlayer(Player *player);

/**
 * @brief Move the player to the left
 *
 * @param player The player structure
 */
void playerMoveLeft(Player *player);

/**
 * @brief Move the player to the right
 *
 * @param player The player structure
 */
void playerMoveRight(Player *player);

/**
 * @brief Shoot if possible
 *
 * @param player The player structure
 */
void playerFire(Player *player);

/**
 * @brief Update player shot position
 *
 * @param player The player structure
 */
void updatePlayerShot(Player *player);

/**
 * @brief Check if the player is hit by any alien shot
 * 
 * @param player The player structure
 * @param aliens The aliens structure
 * @return 1 if hit, 0 otherwise
 */
int resolvePlayerHit(Player *player, Aliens *aliens);
