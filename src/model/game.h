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
 * @def ALIENS_HEIGHT_RATIO
 * @brief The ratio of the screen height taken by the aliens
 */
#define ALIENS_HEIGHT_RATIO 0.65

/**
 * @def ALIENS_GRID_HEIGHT_RATIO
 * @brief The ratio of the aliens height taken by the grid
 */
#define ALIENS_GRID_HEIGHT_RATIO 0.6

/**
 * @def GAME_WIDTH_RATIO
 * @brief The ratio of the screen width taken by the game
 */
#define GAME_WIDTH_RATIO 0.9

/**
 * @def ALIENS_SWAY_FACTOR
 * @brief The factor for aliens horizontal movement
 */
#define ALIENS_SWAY_FACTOR 1.1

/**
 * @def ALIEN_HITBOX_WIDTH_RATIO
 * @brief The ratio of the alien width compared to its cell
 */
#define ALIEN_HITBOX_WIDTH_RATIO 0.6

/**
 * @def ALIEN_HITBOX_HEIGHT_RATIO
 * @brief The ratio of the alien height compared to its cell
 */
#define ALIEN_HITBOX_HEIGHT_RATIO 0.5

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
 * @def MAX_ALIEN_SHOTS
 * @brief Maximum number of simultaneous alien shots
 */
#define MAX_ALIEN_SHOTS 10

/**
 * @def ALIEN_SHOOT_PROBABILITY
 * @brief Probability that an alien shoots per tick
 */
#define ALIEN_SHOOT_PROBABILITY 0.05

/**
 * @def ALIEN_SHOOT_SPEED
 * @brief The alien shoot speed
 */
#define ALIEN_SHOOT_SPEED 0.008

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
  double alienShotX[MAX_ALIEN_SHOTS];   //!< The x coords of alien shots
  double alienShotY[MAX_ALIEN_SHOTS];   //!< The y coords of alien shots
  int alienShotActive[MAX_ALIEN_SHOTS]; //!< Whether each alien shot is active
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
