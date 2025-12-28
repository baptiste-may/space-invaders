/**
 * @file score.h
 * @brief Management of game scores (current and best)
 */

#pragma once

/**
 * @brief Filename where the best score is stored
 */
#define BEST_SCORE_FILE "best_score.dat"

/**
 * @struct _Scores
 * @brief Represents the current and best scores
 */
typedef struct _Scores {
  unsigned current; //!< The current score of the player
  unsigned best;    //!< The best score ever achieved
} Scores;

/**
 * @brief Retrieve the best score from the storage file
 *
 * @return The best score, or 0 if no file exists
 */
unsigned getBestScore();

/**
 * @brief Update the best score if the current score is higher
 *
 * @param scores Pointer to the scores structure to update
 */
void updateBestScore(Scores *scores);
