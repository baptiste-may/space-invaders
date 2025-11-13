#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <stdbool.h>

/**
 * @struct MainMenu
 * @brief The informations about the main menu
 */
typedef struct {
  bool isOpen;  //!< Is the main menu open
  int selected; //!< The selected option
} MainMenu;

#endif
