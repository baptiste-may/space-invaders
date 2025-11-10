#include "ncurses-view.h"
#include <ncurses.h>

int maxWidth, maxHeight;
WINDOW *mainMenu;
int mainMenuSelection = 0;

void drawMainMenu() {
  wclear(mainMenu);
  wattrset(mainMenu, A_UNDERLINE);
  mvwprintw(mainMenu, 0, 1, "Space Inviders");
  wattrset(mainMenu, mainMenuSelection == 0 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 2, 5, "Play");
  wattrset(mainMenu, mainMenuSelection == 1 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 3, 3, "Settings");
  wattrset(mainMenu, mainMenuSelection == 2 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 4, 5, "Quit");
  wrefresh(mainMenu);
}

void initViewNcurses() {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);

  getmaxyx(stdscr, maxHeight, maxWidth);

  int width = 16;
  int height = 7;

  mainMenu =
      newwin(height, width, (maxHeight - height) / 2, (maxWidth - width) / 2);
  box(mainMenu, 0, 0);
  refresh();
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
    case '\n':
      if (mainMenuSelection == 2)
        closeApp = TRUE;
      break;
    case 'z':
    case KEY_UP:
      mainMenuSelection--;
      if (mainMenuSelection < 0)
        mainMenuSelection = 2;
      drawMainMenu();
      break;
    case 's':
    case KEY_DOWN:
      mainMenuSelection = (mainMenuSelection + 1) % 3;
      drawMainMenu();
      break;
    }
  }
}
