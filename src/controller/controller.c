#include "controller.h"
#include "../views/views.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
  Menu *creditsMenu = &(model->creditsMenu);
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

    if (creditsMenu->isOpen) {
      if (handleMenuInput(creditsMenu, event)) {
        if (creditsMenu->selected == 0)
          system("xdg-open https://github.com/baptiste-may/space-invaders");
        else {
          controller->view->destroyCreditsMenu();
          creditsMenu->isOpen = false;
          controller->view->createMainMenu(controller);
          mainMenu->selected = 0;
          mainMenu->isOpen = true;
        }
      }
      controller->view->updateCreditsMenu(controller);
      continue;
    }

    if (mainMenu->isOpen) {
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
          controller->view->destroyMainMenu();
          mainMenu->isOpen = false;
          controller->view->createCreditsMenu(controller);
          creditsMenu->selected = 0;
          creditsMenu->isOpen = true;
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

    // Handle game over menu
    if (gameOverMenu->isOpen) {
      if (handleMenuInput(gameOverMenu, event)) {
        controller->view->destroyGameOverMenu();
        freeGame(model->currentGame);
        model->currentGame = NULL;
        gameOverMenu->isOpen = false;
        if (gameOverMenu->selected == 0) {
          // Restart: create a new game
          startGame(model);
          controller->view->updateGame(controller);
        } else {
          // Main Menu : return to the main menu
          controller->view->destroyGame();
          controller->view->createMainMenu(controller);
          mainMenu->selected = 0;
          mainMenu->isOpen = true;
        }
      }

      controller->view->updateGameOverMenu(controller);
      continue;
    }

    Game *game = model->currentGame;
    if (game != NULL) {
      if (game->gameOver) {
        controller->view->createGameOverMenu(controller);
        gameOverMenu->selected = 0;
        gameOverMenu->isOpen = true;
        continue;
      }
      if (event & EVENT_KEY_ESCAPE) {
        controller->view->createMainMenu(controller);
        mainMenu->selected = 0;
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
