#pragma once

/**
 * @def SHIELD_WIDTH
 * @brief The width of a shield in blocks
 */
#define SHIELD_WIDTH 11

/**
 * @def SHIELD_HEIGHT
 * @brief The height of a shield in blocks
 */
#define SHIELD_HEIGHT 6

/**
 * @enum ShieldBlock
 * @brief The state of a block in a shield
 */
typedef enum _ShieldBlock {
  EMPTY,            //!< No block
  FULL,             //!< A full block
  HALF_FULL_LEFT,   //!< A half full block on the left
  HALF_FULL_RIGHT,  //!< A half full block on the right
  HALF_EMPTY_LEFT,  //!< A half empty block on the left
  HALF_EMPTY_RIGHT, //!< A half empty block on the right
  DESTROYED_LEFT,   //!< A destroyed block on the left
  DESTROYED_RIGHT   //!< A destroyed block on the right
} ShieldBlock;

/**
 * @typedef Shield
 * @brief A shield is a 2D array of ShieldBlocks
 */
typedef ShieldBlock Shield[SHIELD_HEIGHT][SHIELD_WIDTH];

/**
 * @struct Shields
 * @brief The structure representing multiple shields
 */
typedef struct _Shields {
  unsigned nb;    //!< The number of shields
  Shield *blocks; //!< The array of shields
} Shields;

/**
 * @brief Create the shields
 *
 * @param nbShields The number of shields to create
 * @return An array of Shield
 */
Shield *createShields(unsigned nbShields);

/**
 * @brief Free the shields structure
 *
 * @param shields The shields structure to free
 */
void freeShields(Shields *shields);