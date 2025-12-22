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

  // Print game
  if (controller->model->currentGame != NULL) {
    Game *game = controller->model->currentGame;

    // Score and lives
    mvwprintw(gameWin, 1, 2, "Score: %d", game->score);
    mvwprintw(gameWin, 1, maxWidth - 10, "Lives: %d", game->lives);

    // Player
    int playerPosition = game->playerPosition * (maxWidth - 7) + 3;
    mvwprintw(gameWin, maxHeight - 3, playerPosition - 1, "/ \\");
    mvwprintw(gameWin, maxHeight - 2, playerPosition - 2, "[___]");

    // Aliens
    double gridHeight =
        maxHeight * ALIENS_HEIGHT_RATIO * ALIENS_GRID_HEIGHT_RATIO;
    double moveRangeY =
        maxHeight * ALIENS_HEIGHT_RATIO * (1.0 - ALIENS_GRID_HEIGHT_RATIO);

    for (unsigned i = 0; i < game->nbAlienRows; i++) {
      for (unsigned j = 0; j < game->nbAliens; j++) {
        unsigned k = j + i * game->nbAliens;
        int alienIndex = game->aliens[k];

        // Do not display dead aliens
        if (alienIndex < 0)
          continue;

        int alienX = (int)(
            ((maxWidth * GAME_WIDTH_RATIO) / (double)(game->nbAliens)) *
                (j + 0.5) +
            game->aliensX *
                ((maxWidth * ALIENS_SWAY_FACTOR) / (double)(game->nbAliens)));
        int alienY = (int)(
            (gridHeight / (double)(game->nbAlienRows)) * (i + 0.5) +
            maxHeight * HEADER_HEIGHT_RATIO + game->aliensY * moveRangeY);

        switch (alienIndex) {
          case 0:
          case 1:
            mvwprintw(gameWin, alienY - 1, alienX - 2, " _^_");
            mvwprintw(gameWin, alienY, alienX - 2, "(- -)");
            mvwprintw(gameWin, alienY + 1, alienX - 2, " %c %c", alienIndex == 0 ? '/' : '\\', alienIndex == 0 ? '\\' : '/');
            break;
          case 2:
          case 3:
            mvwprintw(gameWin, alienY - 1, alienX - 2, "%1$c\\_/%1$c", alienIndex == 2 ? '|' : ' ');
            mvwprintw(gameWin, alienY, alienX - 2, "%co_o%c", alienIndex == 2 ? '\\' : '/', alienIndex == 2 ? '/' : '\\');
            mvwprintw(gameWin, alienY + 1, alienX - 2, "%1$c%2$c %3$c%1$c", alienIndex == 2 ? ' ' : '|', alienIndex == 2 ? '/' : '\\', alienIndex == 2 ? '\\' : '/');
            break;
          case 4:
          case 5:
            mvwprintw(gameWin, alienY - 1, alienX - 1, " ^");
            mvwprintw(gameWin, alienY, alienX - 1, "/_\\");
            mvwprintw(gameWin, alienY + 1, alienX - 1, "%c %c", alienIndex == 4 ? '/' : '\\', alienIndex == 4 ? '\\' : '/');
            break;
        }
      }
    }

    // Player shoot
    int playerShootX = game->playerShootX * (maxWidth - 7) + 3;
    int playerShootY = game->playerShootY * maxHeight - 1;
    mvwprintw(gameWin, playerShootY, playerShootX, "|");
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
  case 'q':
  case KEY_LEFT:
    return EVENT_KEY_LEFT;
  case 'd':
  case KEY_RIGHT:
    return EVENT_KEY_RIGHT;
  case KEY_RESIZE:
    return EVENT_RESIZE;
  case '\n':
    return EVENT_KEY_ENTER;
  case 27:
    return EVENT_KEY_ESCAPE;
  case ' ':
    return EVENT_KEY_SPACE;
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
