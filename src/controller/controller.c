#include "controller.h"
#include "../views/views.h"

#include <ncurses.h>
#include <stdbool.h>

void mainLoop(Controller *controller) {
  Model *model = controller->model;
  MainMenu *mainMenu = &(model->mainMenu);

  bool closeApp = false;
  Event event;
  while (closeApp == false) {
    event = scanEvent(controller->view);

    if (event & EVENT_RESIZE)
      resize(controller);

    if (event & EVENT_CLOSE) {
      closeApp = true;
      continue;
    }

    if (mainMenu->isOpen == true) {
      if (event & EVENT_KEY_ENTER) {
        switch (mainMenu->selected) {
        case 0:
          // Play or Continue
          destroyMainMenu(controller->view);
          mainMenu->isOpen = false;
          if (model->currentGame == NULL) {
            startGame(model);
            createGame(controller);
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
          closeApp = true;
          break;
        }
      } else if (event & EVENT_KEY_UP) {
        mainMenu->selected--;
        if (mainMenu->selected < 0)
          mainMenu->selected = 3;
      } else if (event & EVENT_KEY_DOWN)
        mainMenu->selected = (mainMenu->selected + 1) % 4;

      updateMainMenu(controller);
      continue;
    }

    if (model->currentGame != NULL) {
      Game *game = model->currentGame;

      // Gérer le menu game over
      if (game->gameOver) {
        if (event & EVENT_KEY_UP) {
          model->gameOverSelected = (model->gameOverSelected - 1 + 2) % 2;
          updateGame(controller);
        } else if (event & EVENT_KEY_DOWN) {
          model->gameOverSelected = (model->gameOverSelected + 1) % 2;
          updateGame(controller);
        } else if (event & EVENT_KEY_ENTER) {
          if (model->gameOverSelected == 0) {
            // Restart : créer une nouvelle partie
            freeGame(model->currentGame);
            startGame(model);
            model->gameOverSelected = 0;
            updateGame(controller);
          } else {
            // Main Menu : retourner au menu principal
            freeGame(model->currentGame);
            model->currentGame = NULL;
            model->gameOverSelected = 0;
            createMainMenu(controller);
            mainMenu->isOpen = true;
          }
        }
        continue;
      }

      // Jeu normal
      if (event & EVENT_KEY_ESCAPE) {
        createMainMenu(controller);
        mainMenu->isOpen = true;
      } else {
        if (event & EVENT_KEY_LEFT)
          playerMoveLeft(game->player);
        if (event & EVENT_KEY_RIGHT)
          playerMoveRight(game->player);
        if (event & EVENT_KEY_SPACE)
          playerFire(game->player, game->aliens);

        nextFrame(game);
        updateGame(controller);
      }
    }
  }
}
