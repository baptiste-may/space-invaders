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
    event = scanEvent(controller);

    if (event == EVENT_RESIZE) {
      resize(controller);
      continue;
    }

    if (event == EVENT_CLOSE) {
      closeApp = true;
      continue;
    }

    if (mainMenu->isOpen == true) {
      switch (event) {
      case EVENT_KEY_ENTER:
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
        break;
      case EVENT_KEY_UP:
        mainMenu->selected--;
        if (mainMenu->selected < 0)
          mainMenu->selected = 3;
        break;
      case EVENT_KEY_DOWN:
        mainMenu->selected = (mainMenu->selected + 1) % 4;
        break;
      default:
        break;
      }
      updateMainMenu(controller);
      continue;
    }

    if (model->currentGame != NULL) {
      switch (event) {
      case EVENT_KEY_ESCAPE:
        createMainMenu(controller);
        mainMenu->isOpen = true;
        break;
      case EVENT_KEY_UP:
        model->currentGame->score++;
        break;
      default:
        break;
      }
      updateGame(controller);
    }
  }
}
