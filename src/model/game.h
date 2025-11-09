#ifndef GAME_H
#define GAME_H

/**
 * @def DEFAULT_LIVES
 * @brief The number of lives when starting a game
 */
#define DEFAULT_LIVES 3

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
} Game;

/**
 * @brief Create game structure
 *
 * @param nbAliens The number of aliens for each row
 * @param nbAlienRows The number of alien rows
 * @param nbBuildings The number of buildings between the aliens and the player
 */
Game *createGame(unsigned nbAliens, unsigned nbAlienRows, unsigned nbBuildings);

/**
 * @brief Free game structure
 *
 * @param game The game structure
 */
void freeGame(Game *game);

#endif
