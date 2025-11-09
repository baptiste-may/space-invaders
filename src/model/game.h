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

Game *createGame(unsigned nbAliens, unsigned nbAlienRows, unsigned nbBuildings);
void freeGame(Game *game);

#endif
