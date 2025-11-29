#pragma once

/**
 * @def DEFAULT_LIVES
 * @brief The number of lives when starting a game
 */
#define DEFAULT_LIVES 3

#define PLAYER_SPEED 0.005

#define SHOOT_SPEED 0.01

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
  char *aliens; //!< The grid of aliens where each number is a variety of alien

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

void nextFrame(Game *game);

void movePlayerLeft(Game *game);
void movePlayerRight(Game *game);
void playerShoot(Game *game);
