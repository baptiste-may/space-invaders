#include "ncurses-view.h"
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

int maxWidth, maxHeight;

// Main Menu
WINDOW *mainMenu = NULL;
const char mainMenuTitle[] = "Space Invaders";
const unsigned mainMenuWidth = 18, mainMenuHeight = 8, nbMainMenuButtons = 4;
int mainMenuSelection = 0;

// Game Window
WINDOW *gameWin = NULL;

void deleteMainMenu() {
  delwin(mainMenu);
  mainMenu = NULL;
}

void drawMainMenu() {
  wclear(mainMenu);
  mvwin(mainMenu, (maxHeight - mainMenuHeight) / 2,
        (maxWidth - mainMenuWidth) / 2);
  box(mainMenu, 0, 0);
  mvwprintw(mainMenu, 0, (mainMenuWidth - strlen(mainMenuTitle)) / 2,
            mainMenuTitle);

  wattrset(mainMenu, mainMenuSelection == 0 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 2, gameWin == NULL ? 7 : 5,
            gameWin == NULL ? "Play" : "Continue");

  wattrset(mainMenu, mainMenuSelection == 1 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 3, 5, "Settings");

  wattrset(mainMenu, mainMenuSelection == 2 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 4, 6, "Credit");

  wattrset(mainMenu, mainMenuSelection == 3 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 5, 7, "Quit");

  wattrset(mainMenu, A_NORMAL);
  wrefresh(mainMenu);
}

void createMainMenu() {
  mainMenu =
      newwin(mainMenuHeight, mainMenuWidth, (maxHeight - mainMenuHeight) / 2,
             (maxWidth - mainMenuWidth) / 2);
  mainMenuSelection = 0;
  drawMainMenu();
}

void drawGameWindow(Model *model) {
  wclear(gameWin);
  wresize(gameWin, maxHeight, maxWidth);
  box(gameWin, 0, 0);

  if (model->currentGame != NULL) {
    Game *game = model->currentGame;

    mvwprintw(gameWin, 2, 2, "Score: %d", game->score);
    mvwprintw(gameWin, 2, maxWidth - 10, "Lives: %d", game->lives);
  }

  wrefresh(gameWin);
}

void createGameWindow(Model *model) {
  gameWin = newwin(maxHeight, maxWidth, 0, 0);
  drawGameWindow(model);
}

void initViewNcurses() {
  initscr();
  noecho();
  halfdelay(1);
  curs_set(0);
  keypad(stdscr, TRUE);

  getmaxyx(stdscr, maxHeight, maxWidth);
  refresh();

  createMainMenu();
}

void closeViewNcurses() {
  delwin(mainMenu);
  mainMenu = NULL;
  delwin(gameWin);
  gameWin = NULL;
  endwin();
}

void loopViewNcurses(Model *model) {
  bool closeApp = FALSE;
  int key;
  while (closeApp == FALSE) {
    key = getch();

    if (key == KEY_RESIZE) {
      getmaxyx(stdscr, maxHeight, maxWidth);
      if (mainMenu != NULL)
        drawMainMenu();
      if (gameWin != NULL)
        drawGameWindow(model);
      continue;
    }

    if (mainMenu != NULL) {
      switch (key) {
      case '\n':
        switch (mainMenuSelection) {
        case 0:
          // Play or Continue
          deleteMainMenu();
          if (gameWin == NULL) {
            startGame(model);
            createGameWindow(model);
          }
          break;
        case 1:
          // Settings
          break;
        case 2:
          // About
          break;
        case 3:
          // Exit
          closeApp = TRUE;
          break;
        }
        break;
      case 'z':
      case KEY_UP:
        mainMenuSelection--;
        if (mainMenuSelection < 0)
          mainMenuSelection = nbMainMenuButtons - 1;
        break;
      case 's':
      case KEY_DOWN:
        mainMenuSelection = (mainMenuSelection + 1) % nbMainMenuButtons;
        break;
      }
      drawMainMenu();
      continue;
    }

    if (gameWin != NULL) {
      switch (key) {
      case 27:
        createMainMenu();
        break;
      }
      drawGameWindow(model);
      continue;
    }
  }
}
