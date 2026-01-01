#pragma once

#include <stdbool.h>

typedef struct {
  bool isOpen;       //!< Is the menu open
  unsigned selected; //!< The selected option
  unsigned nbElts;   //!< Number of elements in the menu
} Menu;

Menu createMenu(unsigned nbElts);
void nextMenuElt(Menu *menu);
void previousMenuElt(Menu *menu);
