#include "ncurses-view.h"
#include "../controller/controller.h"
#include "views.h"
#include <math.h>
#include <ncurses.h>
#include <string.h>

int maxWidth, maxHeight;

// Main Menu
WINDOW *mainMenu = NULL;
const char mainMenuTitle[] = "Space Invaders";
const unsigned mainMenuWidth = 18, mainMenuHeight = 7;

// Game over Menu
WINDOW *gameOverMenu = NULL;
const char gameOverTitle[] = "GAME OVER";
const unsigned gameOverWidth = 18, gameOverHeight = 7;

// Game Window
WINDOW *gameWin = NULL;

static void destroyMainMenuNcurses() {
  if (mainMenu != NULL) {
    wclear(mainMenu);
    wrefresh(mainMenu);
    delwin(mainMenu);
    mainMenu = NULL;
    if (gameWin != NULL) {
      touchwin(gameWin);
      wrefresh(gameWin);
    } else {
      touchwin(stdscr);
      refresh();
    }
  }
}

static void destroyGameOverNcurses() {
  if (gameOverMenu != NULL) {
    wclear(gameOverMenu);
    wrefresh(gameOverMenu);
    delwin(gameOverMenu);
    gameOverMenu = NULL;
    if (gameWin != NULL) {
      wclear(gameWin);
      wrefresh(gameWin);
    } else {
      touchwin(stdscr);
      refresh();
    }
  }
}

static void updateMainMenuNcurses(Controller *controller) {
  unsigned selected = controller->model->mainMenu.selected;

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
  mvwprintw(mainMenu, 3, 6, "Credit");

  wattrset(mainMenu, selected == 2 ? A_REVERSE : A_NORMAL);
  mvwprintw(mainMenu, 4, 7, "Quit");

  wattrset(mainMenu, A_NORMAL);
  wrefresh(mainMenu);
}

static void updateGameOverMenuNcurses(Controller *controller) {
  unsigned selected = controller->model->gameOverMenu.selected;

  wclear(gameOverMenu);
  mvwin(gameOverMenu, (maxHeight - gameOverHeight) / 2,
        (maxWidth - gameOverWidth) / 2);
  box(gameOverMenu, 0, 0);

  if (controller->model->currentGame == NULL)
    return;
  Scores scores = controller->model->currentGame->scores;

  mvwprintw(gameOverMenu, 0, (gameOverWidth - strlen(gameOverTitle)) / 2,
            gameOverTitle);

  int currentScoreLen =
      13 + (scores.current == 0 ? 1 : (int)floor(log10(scores.current)) + 1);
  mvwprintw(gameOverMenu, 2, (gameOverWidth - currentScoreLen) / 2,
            "Final Score: %d", scores.current);

  int bestScoreLen =
      12 + (scores.best == 0 ? 1 : (int)floor(log10(scores.best)) + 1);
  mvwprintw(gameOverMenu, 3, (gameOverWidth - bestScoreLen) / 2,
            "Best Score: %d", scores.best);

  wattrset(gameOverMenu, selected == 0 ? A_REVERSE : A_NORMAL);
  mvwprintw(gameOverMenu, 4, (gameOverWidth - 7) / 2, "Restart");

  wattrset(gameOverMenu, selected == 1 ? A_REVERSE : A_NORMAL);
  mvwprintw(gameOverMenu, 5, (gameOverWidth - 9) / 2, "Main Menu");

  wattrset(gameOverMenu, A_NORMAL);
  wrefresh(gameWin);
  wrefresh(gameOverMenu);
}

static void createMainMenuNcurses(Controller *controller) {
  if (mainMenu != NULL)
    delwin(mainMenu);
  mainMenu =
      newwin(mainMenuHeight, mainMenuWidth, (maxHeight - mainMenuHeight) / 2,
             (maxWidth - mainMenuWidth) / 2);
  updateMainMenuNcurses(controller);
}

static void createGameOverMenuNcurses(Controller *controller) {
  if (gameOverMenu != NULL)
    delwin(gameOverMenu);
  gameOverMenu =
      newwin(gameOverHeight, gameOverWidth, (maxHeight - gameOverHeight) / 2,
             (maxWidth - gameOverWidth) / 2);
  updateGameOverMenuNcurses(controller);
}

static void destroyGameNcurses() {
  if (gameWin != NULL) {
    wclear(gameWin);
    wrefresh(gameWin);
    delwin(gameWin);
    gameWin = NULL;
    touchwin(stdscr);
    refresh();
  }
}

static void updateGameNcurses(Controller *controller) {
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

    if (game->playerDeathFrame >= 0 &&
        game->playerDeathFrame < PLAYER_DEATH_FRAMES) {
      if (game->playerDeathFrame < PLAYER_DEATH_FRAMES / 2) {
        mvwprintw(gameWin, maxHeight - 3, playerPosition - 1, "\\ /");
        mvwprintw(gameWin, maxHeight - 2, playerPosition - 2, "-- --");
      }
    } else {
      mvwprintw(gameWin, maxHeight - 3, playerPosition - 1, "/ \\");
      mvwprintw(gameWin, maxHeight - 2, playerPosition - 2, "[___]");
    }

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

static void createGameNcurses(Controller *controller) {
  if (gameWin != NULL)
    delwin(gameWin);
  gameWin = newwin(maxHeight, maxWidth, 0, 0);
  updateGameNcurses(controller);
}

static void initViewNcurses(Controller *controller) {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  getmaxyx(stdscr, maxHeight, maxWidth);
  refresh();

  createMainMenuNcurses(controller);
}

static void closeViewNcurses() {
  destroyMainMenuNcurses();
  destroyGameNcurses();
  endwin();
}

static Event scanEventNcurses() {
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

static void resizeNcurses(Controller *controller) {
  getmaxyx(stdscr, maxHeight, maxWidth);
  if (mainMenu != NULL)
    updateMainMenuNcurses(controller);
  if (gameWin != NULL)
    updateGameNcurses(controller);
}

ViewInterface getNcursesInterface() {
  return (ViewInterface){initViewNcurses,
                         closeViewNcurses,
                         scanEventNcurses,
                         createMainMenuNcurses,
                         updateMainMenuNcurses,
                         destroyMainMenuNcurses,
                         createGameNcurses,
                         updateGameNcurses,
                         destroyGameNcurses,
                         createGameOverMenuNcurses,
                         updateGameOverMenuNcurses,
                         destroyGameOverNcurses,
                         resizeNcurses};
}
