#pragma once
#include "shield.h"
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
#define ALIENS_HEIGHT_RATIO 0.55

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
 * @def UFO_HEIGHT_RATIO
 * @brief The Y position of the UFO (below header, above aliens)
 */
#define UFO_HEIGHT_RATIO 0.12

/**
 * @def UFO_SPEED
 * @brief The UFO horizontal speed
 */
#define UFO_SPEED 0.0001

/**
 * @def UFO_SPAWN_CHANCE
 * @brief Probability that UFO spawns per tick (when not active)
 */
#define UFO_SPAWN_CHANCE 0.001

/**
 * @def EXPLOSION_FRAMES
 * @brief Number of frames during an explosion
 */
#define EXPLOSION_FRAMES 20

/**
 * @struct Aliens
 * @brief The structure representing the aliens group
 */
typedef struct {
  unsigned nbAliens;    //!< The number of aliens for each row
  unsigned nbAlienRows; //!< The number of alien rows
  int *aliens; //!< The grid of aliens where each number is a variety of alien
  double aliensX;       //!< The x coords of the most top left alien
  double aliensY;       //!< The y coords of the most top left alien
  double alienMovement; //!< The movement of the aliens

  double alienShotX[MAX_ALIEN_SHOTS];    //!< The x coords of alien shots
  double alienShotY[MAX_ALIEN_SHOTS];    //!< The y coords of alien shots
  bool alienShotActive[MAX_ALIEN_SHOTS]; //!< Whether each alien shot is active

  // UFO
  bool ufoActive;   //!< Whether the UFO is currently on screen
  double ufoX;      //!< The x coordinate of the UFO (0-1)
  int ufoDirection; //!< The direction of UFO movement (-1 left, 1 right)
  unsigned
      shotCounter; //!< Counter for shots fired (for UFO points calculation)
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
 * @param shields The shields structure
 */
void updateAlienShots(Aliens *aliens, Shields *shields);

/**
 * @brief Animate aliens (change sprites)
 *
 * @param aliens The aliens structure
 * @param isTick The current frame is on a tick
 */
void animateAliens(Aliens *aliens, bool isTick);

/**
 * @brief Check if a shot hits an alien and handle the destruction
 *
 * @param aliens The aliens structure
 * @param shotX The x coordinate of the shot (normalized 0-1)
 * @param shotY The y coordinate of the shot (normalized 0-1)
 * @return The points earned (0 if no hit)
 */
int resolveAlienHit(Aliens *aliens, double shotX, double shotY);

/**
 * @brief Update UFO position and spawn logic
 *
 * @param aliens The aliens structure
 */
void updateUFO(Aliens *aliens);

/**
 * @brief Check if a shot hits the UFO
 *
 * @param aliens The aliens structure
 * @param shotX The x coordinate of the shot (normalized 0-1)
 * @param shotY The y coordinate of the shot (normalized 0-1)
 * @return The points earned (0 if no hit)
 */
int resolveUFOHit(Aliens *aliens, double shotX, double shotY);

/**
 * @brief Increment shot counter (called when player shoots)
 *
 * @param aliens The aliens structure
 */
void incrementShotCounter(Aliens *aliens);
