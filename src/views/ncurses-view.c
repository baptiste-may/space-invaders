#include "ncurses-view.h"
#include <ncurses.h>
#include <string.h>

int maxWidth, maxHeight;

// Main Menu
WINDOW *mainMenu = NULL;
const char mainMenuTitle[] = "Space Invaders";
const unsigned mainMenuWidth = 18, mainMenuHeight = 8;

// Game Window
WINDOW *gameWin = NULL;

void destroyMainMenuNcurses() {
  delwin(mainMenu);
  mainMenu = NULL;
}

void updateMainMenuNcurses(Controller *controller) {
  int selected = controller->model->mainMenu.selected;

  wclear(mainMenu);
  mvwin(mainMenu, (maxHeight - mainMenuHeight) / 2,
        (maxWidth - mainMenuWidth) / 2);
  box(mainMenu, 0, 0);
  mvwprintw(mainMenu, 0, (mainMenuWidth - strlen(mainMenuTitle)) / 2,
            mainMenuTitle);

  wattrset(mainMenu, selected == 0 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 2, gameWin == NULL ? 7 : 5,
            gameWin == NULL ? "Play" : "Continue");

  wattrset(mainMenu, selected == 1 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 3, 5, "Settings");

  wattrset(mainMenu, selected == 2 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 4, 6, "Credit");

  wattrset(mainMenu, selected == 3 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 5, 7, "Quit");

  wattrset(mainMenu, A_NORMAL);
  wrefresh(mainMenu);
}

void createMainMenuNcurses(Controller *controller) {
  mainMenu =
      newwin(mainMenuHeight, mainMenuWidth, (maxHeight - mainMenuHeight) / 2,
             (maxWidth - mainMenuWidth) / 2);
  controller->model->mainMenu.selected = 0;
  updateMainMenuNcurses(controller);
}

void destroyGameNcurses() {
  delwin(gameWin);
  gameWin = NULL;
}

void updateGameNcurses(Controller *controller) {
  wclear(gameWin);
  wresize(gameWin, maxHeight, maxWidth);
  box(gameWin, 0, 0);

  if (controller->model->currentGame != NULL) {
    Game *game = controller->model->currentGame;

    mvwprintw(gameWin, 2, 2, "Score: %d", game->score);
    mvwprintw(gameWin, 2, maxWidth - 10, "Lives: %d", game->lives);
  }

  wrefresh(gameWin);
}

void createGameNcurses(Controller *controller) {
  gameWin = newwin(maxHeight, maxWidth, 0, 0);
  updateGameNcurses(controller);
}

void initViewNcurses(Controller *controller) {
  initscr();
  cbreak();
  noecho();
  timeout(10);
  curs_set(0);
  keypad(stdscr, TRUE);

  getmaxyx(stdscr, maxHeight, maxWidth);
  refresh();

  createMainMenuNcurses(controller);
}

void closeViewNcurses() {
  destroyMainMenuNcurses();
  destroyGameNcurses();
  endwin();
}

Event scanEventNcurses() {
  int ch = getch();
  switch (ch) {
  case ERR:
    return NO_EVENT;
  case 'z':
  case KEY_UP:
    return EVENT_KEY_UP;
  case 's':
  case KEY_DOWN:
    return EVENT_KEY_DOWN;
  case KEY_RESIZE:
    return EVENT_RESIZE;
  case '\n':
    return EVENT_KEY_ENTER;
  case 27:
    return EVENT_KEY_ESCAPE;
  }
  return NO_EVENT;
}

void resizeNcurses(Controller *controller) {
  getmaxyx(stdscr, maxHeight, maxWidth);
  if (mainMenu != NULL)
    updateMainMenuNcurses(controller);
  if (gameWin != NULL)
    updateGameNcurses(controller);
}
