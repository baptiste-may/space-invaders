#include "controller.h"
#include "../views/views.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

bool handleMenuInput(Menu *menu, Event e) {
  if (e & EVENT_KEY_ENTER)
    return true;
  if (e & EVENT_KEY_DOWN)
    nextMenuElt(menu);
  if (e & EVENT_KEY_UP)
    previousMenuElt(menu);
  return false;
}

void mainLoop(Controller *controller) {
  Model *model = controller->model;
  Menu *mainMenu = &(model->mainMenu);
  Menu *gameOverMenu = &(model->gameOverMenu);

  bool closeApp = false;
  Event event;
  while (closeApp == false) {
    event = controller->view->scanEvent();

    if (event & EVENT_RESIZE)
      controller->view->resize(controller);

    if (event & EVENT_CLOSE) {
      closeApp = true;
      continue;
    }

    if (mainMenu->isOpen == true) {
      if (handleMenuInput(mainMenu, event)) {
        switch (mainMenu->selected) {
        case 0:
          // Play or Continue
          controller->view->destroyMainMenu();
          mainMenu->isOpen = false;
          if (model->currentGame == NULL) {
            startGame(model);
            controller->view->createGame(controller);
          }
          break;
        case 1:
          // About
          break;
        case 2:
          // Exit
          closeApp = true;
          break;
        }
      }

      controller->view->updateMainMenu(controller);
      continue;
    }

    if (model->currentGame != NULL) {
      Game *game = model->currentGame;

      // Handle game over menu
      if (game->gameOver) {
        if (handleMenuInput(gameOverMenu, event)) {
          gameOverMenu->isOpen = false;
          freeGame(model->currentGame);
          model->currentGame = NULL;
          if (gameOverMenu->selected == 0) {
            // Restart: create a new game
            startGame(model);
            controller->view->updateGame(controller);
          } else {
            // Main Menu: return to main menu
            mainMenu->isOpen = true;
            controller->view->createMainMenu(controller);
          }
          gameOverMenu->selected = 0;
          continue;
        }
      }

      // Normal gameplay
      if (event & EVENT_KEY_ESCAPE) {
        controller->view->createMainMenu(controller);
        mainMenu->isOpen = true;
      } else {
        // Block all inputs if player is exploding
        if (game->playerDeathFrame < 0) {
          if (event & EVENT_KEY_LEFT)
            playerMoveLeft(game->player);
          if (event & EVENT_KEY_RIGHT)
            playerMoveRight(game->player);
          if (event & EVENT_KEY_SPACE)
            playerFire(game->player, game->aliens);
        }
        nextFrame(game);
        controller->view->updateGame(controller);
      }
    }
  }
}
