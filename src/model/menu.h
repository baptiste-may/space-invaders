#pragma once

#include <stdbool.h>

typedef struct {
  bool isOpen;       //!< Is the menu open
  unsigned selected; //!< The selected option
  unsigned nbElts;   //!< Number of elements in the menu
} Menu;

/**
 * @brief Create a new menu
 *
 * @param nbElts The number of elements in the menu
 * @return The created menu
 */
Menu createMenu(unsigned nbElts);

/**
 * @brief Select the next element in the menu (cyclic)
 *
 * @param menu The menu structure
 */
void nextMenuElt(Menu *menu);

/**
 * @brief Select the previous element in the menu (cyclic)
 *
 * @param menu The menu structure
 */
void previousMenuElt(Menu *menu);
