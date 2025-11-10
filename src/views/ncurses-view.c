#include "ncurses-view.h"
#include <ncurses.h>
#include <string.h>

int maxWidth, maxHeight;

WINDOW *mainMenu;
const char mainMenuTitle[] = "Space Inviders";
const unsigned mainMenuWidth = 18, mainMenuHeight = 7, nbMainMenuButtons = 4;
const char *mainMenuButtons[] = {"Play", "Settings", "About", "Quit"};
int mainMenuSelection = 0;

void drawMainMenu() {
  wclear(mainMenu);
  mvwin(mainMenu, (maxHeight - mainMenuHeight) / 2,
        (maxWidth - mainMenuWidth) / 2);
  refresh();
  box(mainMenu, 0, 0);
  mvwprintw(mainMenu, 0, (mainMenuWidth - strlen(mainMenuTitle)) / 2,
            mainMenuTitle);
  for (int i = 0; i < nbMainMenuButtons; i++) {
    wattrset(mainMenu, mainMenuSelection == i ? A_REVERSE : A_NORMAL);
    mvwprintw(mainMenu, i + 2, (mainMenuWidth - strlen(mainMenuButtons[i])) / 2,
              "%s", mainMenuButtons[i]);
  }
  wattrset(mainMenu, A_NORMAL);
  wrefresh(mainMenu);
}

void initViewNcurses() {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);

  getmaxyx(stdscr, maxHeight, maxWidth);
  refresh();

  mainMenu =
      newwin(mainMenuHeight, mainMenuWidth, (maxHeight - mainMenuHeight) / 2,
             (maxWidth - mainMenuWidth) / 2);
  drawMainMenu();
}

void closeViewNcurses() {
  delwin(mainMenu);
  endwin();
}

void loopViewNcurses(Model *model) {
  bool closeApp = FALSE;
  while (closeApp == FALSE) {
    switch (getch()) {
    case KEY_RESIZE:
      getmaxyx(stdscr, maxHeight, maxWidth);
      drawMainMenu();
      break;
    case '\n':
      if (mainMenuSelection == nbMainMenuButtons - 1)
        closeApp = TRUE;
      break;
    case 'z':
    case KEY_UP:
      mainMenuSelection--;
      if (mainMenuSelection < 0)
        mainMenuSelection = nbMainMenuButtons - 1;
      drawMainMenu();
      break;
    case 's':
    case KEY_DOWN:
      mainMenuSelection = (mainMenuSelection + 1) % nbMainMenuButtons;
      drawMainMenu();
      break;
    }
  }
}
