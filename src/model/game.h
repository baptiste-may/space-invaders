#pragma once

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
 * @def ALIEN_SPEED_X
 * @brief The aliens X speed (by tick)
 */
#define ALIEN_SPEED_X 0.05

/**
 * @def ALIEN_SPEED_Y
 * @brief The aliens Y speed (everytime they hit the wall)
 */
#define ALIEN_SPEED_Y 0.1

/**
 * @struct Game
 * @brief The structure of a game, including all the informations
 */
typedef struct {
  unsigned score;       //!< The current score
  unsigned lives;       //!< The number of lives from the player
  unsigned nbAliens;    //!< The number of aliens for each row
  unsigned nbAlienRows; //!< The number of alien rows
  unsigned nbBuildings; //!< The number of buildings between the aliens and the
                        //!< player

  unsigned frame;    //!< The current frame of the game (for timing, from 0 to
                     //!< frameMax)
  unsigned frameMax; //!< The max number of frame

  int *aliens; //!< The grid of aliens where each number is a variety of alien
  double aliensX;       //!< The x coords of the most top left alien
  double aliensY;       //!< The y coords of the most top left alien
  double alienMovement; //!< The movement of the aliens

  double playerPosition; //!< The player position in pourcentage
  double playerShootX;   //!< The x coords of the player shoot
  double playerShootY;   //!< The y coords of the player shoot
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
