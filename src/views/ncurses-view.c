#include "ncurses-view.h"
#include <math.h>
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
    const double margin = (1.0 - GAME_WIDTH_RATIO) / 2.0;

    // Scores and lives
    mvwprintw(gameWin, 1, 2, "Score: %d", game->scores.current);
    unsigned bestScoreSize =
        12 + (unsigned)(floor(log10(game->scores.best)) + 1);
    mvwprintw(gameWin, 1, maxWidth / 2 - bestScoreSize / 2, "Best scores: %d",
              game->scores.best);
    mvwprintw(gameWin, 1, maxWidth - 10, "Lives: %d", game->player->lives);

    // Player
    int playerPosition =
        (int)((game->player->position * GAME_WIDTH_RATIO + margin) * maxWidth);
    mvwprintw(gameWin, maxHeight - 3, playerPosition - 1, "/ \\");
    mvwprintw(gameWin, maxHeight - 2, playerPosition - 2, "[___]");

    // Aliens
    double gridHeight =
        maxHeight * ALIENS_HEIGHT_RATIO * ALIENS_GRID_HEIGHT_RATIO;
    double moveRangeY =
        maxHeight * ALIENS_HEIGHT_RATIO * (1.0 - ALIENS_GRID_HEIGHT_RATIO);

    for (unsigned i = 0; i < game->aliens->nbAlienRows; i++) {
      for (unsigned j = 0; j < game->aliens->nbAliens; j++) {
        unsigned k = j + i * game->aliens->nbAliens;
        int alienIndex = game->aliens->aliens[k];

        // Do not display dead aliens
        if (alienIndex < -EXPLOSION_FRAMES)
          continue;

        int alienX = (int)(margin * maxWidth +
                           ((maxWidth * GAME_WIDTH_RATIO) /
                            (double)(game->aliens->nbAliens)) *
                               (j + 0.5) +
                           (game->aliens->aliensX - 0.5) *
                               ((maxWidth * ALIENS_SWAY_FACTOR) /
                                (double)(game->aliens->nbAliens)));
        int alienY = (int)((gridHeight / (double)(game->aliens->nbAlienRows)) *
                               (i + 0.5) +
                           maxHeight * (UFO_HEIGHT_RATIO + 0.05) +
                           game->aliens->aliensY * moveRangeY);

        if (alienIndex < 0) {
          mvwprintw(gameWin, alienY - 1, alienX - 1, "\\ /");
          mvwprintw(gameWin, alienY, alienX - 2, "-- --");
          mvwprintw(gameWin, alienY + 1, alienX - 1, "/ \\");
          continue;
        }

        switch (alienIndex) {
        case 0:
        case 1:
          mvwprintw(gameWin, alienY - 1, alienX - 2, " _^_");
          mvwprintw(gameWin, alienY, alienX - 2, "(- -)");
          mvwprintw(gameWin, alienY + 1, alienX - 2, " %c %c",
                    alienIndex == 0 ? '/' : '\\', alienIndex == 0 ? '\\' : '/');
          break;
        case 2:
        case 3:
          mvwprintw(gameWin, alienY - 1, alienX - 2, "%1$c\\_/%1$c",
                    alienIndex == 2 ? '|' : ' ');
          mvwprintw(gameWin, alienY, alienX - 2, "%co_o%c",
                    alienIndex == 2 ? '\\' : '/', alienIndex == 2 ? '/' : '\\');
          mvwprintw(gameWin, alienY + 1, alienX - 2, "%1$c%2$c %3$c%1$c",
                    alienIndex == 2 ? ' ' : '|', alienIndex == 2 ? '/' : '\\',
                    alienIndex == 2 ? '\\' : '/');
          break;
        case 4:
        case 5:
          mvwprintw(gameWin, alienY - 1, alienX - 1, " ^");
          mvwprintw(gameWin, alienY, alienX - 1, "/_\\");
          mvwprintw(gameWin, alienY + 1, alienX - 1, "%c %c",
                    alienIndex == 4 ? '/' : '\\', alienIndex == 4 ? '\\' : '/');
          break;
        }
      }
    }

    // UFO
    if (game->aliens->ufoActive) {
      int ufoX = (int)(game->aliens->ufoX * maxWidth);
      int ufoY = (int)(UFO_HEIGHT_RATIO * maxHeight);
      mvwprintw(gameWin, ufoY - 1, ufoX - 1, "___");
      mvwprintw(gameWin, ufoY, ufoX - 2, "/___\\");
      mvwprintw(gameWin, ufoY + 1, ufoX - 2, "\\/ \\/");
    }

    // Shields
    const int shieldsY =
        (int)((1 - PLAYER_HEIGHT_RATIO - SHIELD_HEIGHT_RATIO / 2) * maxHeight);
    const int shieldsXDiff = (int)(maxWidth / game->shields.nb);
    for (unsigned n = 0; n < game->shields.nb; n++) {
      const int shieldX = (int)(shieldsXDiff * n + shieldsXDiff / 2);
      for (unsigned i = 0; i < SHIELD_HEIGHT; i++) {
        for (unsigned j = 0; j < SHIELD_WIDTH; j++) {
          ShieldBlock block = game->shields.blocks[n][i][j];
          if (block == EMPTY)
            continue;
          mvwprintw(gameWin, shieldsY - SHIELD_HEIGHT / 2 + i,
                    shieldX - SHIELD_WIDTH / 2 + j, "%c",
                    block == HALF_EMPTY_LEFT || block == HALF_FULL_LEFT ? '\\'
                    : block == HALF_EMPTY_RIGHT || block == HALF_FULL_RIGHT
                        ? '/'
                    : block == FULL ? '#'
                                    : ':');
        }
      }
    }

    // Alien shots
    for (int i = 0; i < MAX_ALIEN_SHOTS; i++) {
      if (game->aliens->alienShotActive[i]) {
        double alienShootX =
            (game->aliens->alienShotX[i] * GAME_WIDTH_RATIO + margin) *
            maxWidth;
        double alienShootY = game->aliens->alienShotY[i] * maxHeight;
        mvwprintw(gameWin, alienShootY, alienShootX, "|");
      }
    }

    // Player shoot
    if (game->player->shootX != -1) {
      int playerShootX =
          (int)((game->player->shootX * GAME_WIDTH_RATIO + margin) * maxWidth);
      int playerShootY = (int)(game->player->shootY * maxHeight);
      mvwprintw(gameWin, playerShootY, playerShootX, "|");
    }
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
  nodelay(stdscr, TRUE);

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
  Event res = NO_EVENT;
  int ch;
  while ((ch = getch()) != ERR) {
    switch (ch) {
    case 'z':
    case KEY_UP:
      res |= EVENT_KEY_UP;
      break;
    case 's':
    case KEY_DOWN:
      res |= EVENT_KEY_DOWN;
      break;
    case 'q':
    case KEY_LEFT:
      res |= EVENT_KEY_LEFT;
      break;
    case 'd':
    case KEY_RIGHT:
      res |= EVENT_KEY_RIGHT;
      break;
    case KEY_RESIZE:
      res |= EVENT_RESIZE;
      break;
    case '\n':
      res |= EVENT_KEY_ENTER;
      break;
    case 27:
      res |= EVENT_KEY_ESCAPE;
      break;
    case ' ':
      res |= EVENT_KEY_SPACE;
      break;
    }
  }
  napms(10);
  return res;
}

void resizeNcurses(Controller *controller) {
  getmaxyx(stdscr, maxHeight, maxWidth);
  if (mainMenu != NULL)
    updateMainMenuNcurses(controller);
  if (gameWin != NULL)
    updateGameNcurses(controller);
}
