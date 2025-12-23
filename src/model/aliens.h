#pragma once

#include <stdbool.h>

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
 * @def MAX_ALIEN_SHOTS
 * @brief Maximum number of simultaneous alien shots
 */
#define MAX_ALIEN_SHOTS 3

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
 * @struct Aliens
 * @brief The structure representing the aliens group
 */
typedef struct {
  unsigned nbAliens;    //!< The number of aliens for each row
  unsigned nbAlienRows; //!< The number of alien rows
  int *aliens;          //!< The grid of aliens where each number is a variety of alien
  double aliensX;       //!< The x coords of the most top left alien
  double aliensY;       //!< The y coords of the most top left alien
  double alienMovement; //!< The movement of the aliens
  
  double alienShotX[MAX_ALIEN_SHOTS];    //!< The x coords of alien shots
  double alienShotY[MAX_ALIEN_SHOTS];    //!< The y coords of alien shots
  bool alienShotActive[MAX_ALIEN_SHOTS]; //!< Whether each alien shot is active
} Aliens;

/**
 * @brief Create aliens structure
 *
 * @param nbAliens The number of aliens for each row
 * @param nbAlienRows The number of alien rows
 * @return The aliens structure
 */
Aliens *createAliens(unsigned nbAliens, unsigned nbAlienRows);

/**
 * @brief Free aliens structure
 *
 * @param aliens The aliens structure
 */
void freeAliens(Aliens *aliens);

/**
 * @brief Move the aliens
 *
 * @param aliens The aliens structure
 * @return if game over (aliens reached bottom)
 */
bool moveAliens(Aliens *aliens);

/**
 * @brief Make aliens shoot randomly
 *
 * @param aliens The aliens structure
 */
void alienShoot(Aliens *aliens);

/**
 * @brief Update alien shots positions
 *
 * @param aliens The aliens structure
 */
void updateAlienShots(Aliens *aliens);

/**
 * @brief Animate aliens (change sprites)
 *
 * @param aliens The aliens structure
 */
void animateAliens(Aliens *aliens);

/**
 * @brief Check if a shot hits an alien and handle the destruction
 * 
 * @param aliens The aliens structure
 * @param shotX The x coordinate of the shot (normalized 0-1)
 * @param shotY The y coordinate of the shot (normalized 0-1)
 * @return The points earned (0 if no hit)
 */
int resolveAlienHit(Aliens *aliens, double shotX, double shotY);
