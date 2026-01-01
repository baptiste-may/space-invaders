#include "menu.h"

Menu createMenu(unsigned int nbElts) { return (Menu){false, 0, nbElts}; }

void nextMenuElt(Menu *menu) {
  menu->selected = (menu->selected + 1) % menu->nbElts;
}

void previousMenuElt(Menu *menu) {
  if (menu->selected == 0)
    menu->selected = menu->nbElts - 1;
  else
    menu->selected--;
}
