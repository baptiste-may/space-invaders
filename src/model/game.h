#pragma once

#include "aliens.h"
#include "player.h"
#include "score.h"
#include "shield.h"
#include <stdbool.h>

/**
 * @def DEFAULT_FRAME_LENGTH
 * @brief The number of frame before calculating the next tick
 */
#define DEFAULT_FRAME_LENGTH 60

/**
 * @def HEADER_HEIGHT_RATIO
 * @brief The ratio of the screen height taken by the header (score and lives)
 */
#define HEADER_HEIGHT_RATIO 0.1

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
 * @def PLAYER_DEATH_FRAMES
 * @brief Number of frames for player death animation
 */
#define PLAYER_DEATH_FRAMES 60

/**
 * @struct Game
 * @brief The structure of a game, including all the informations
 */
typedef struct {
  unsigned frame;    //!< The current frame of the game (for timing, from 0 to
                     //!< frameMax)
  unsigned frameMax; //!< The max number of frame

  Scores scores;   //!< The scores structure
  Aliens *aliens;  //!< The aliens structure
  Player *player;  //!< The player structure
  Shields shields; //!< The shields structure

  bool gameOver;        //!< Game over flag (0 = playing, 1 = game over)
  int playerDeathFrame; //!< Frame counter for player death animation (-1 = not
                        //!< dying)
  int waveTransitionFrame;  //!< Frame counter for wave transition (0 = no
                            //!< transition)
  double currentAlienSpeed; //!< Current alien speed multiplier
} Game;

/**
 * @brief Create game structure
 *
 * @param nbAliens The number of aliens for each row
 * @param nbAlienRows The number of alien rows
 * @param nbShields The number of shields between the aliens and the player
 *
 * @return The game structure
 */
Game *newGame(unsigned nbAliens, unsigned nbAlienRows, unsigned nbShields);

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
