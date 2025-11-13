#include "views.h"
#include "ncurses-view.h"
#include "sdl-view.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ViewType getViewType(const char *arg) {
  if (!strcmp(arg, "ncurses"))
    return NCURSES;
  if (!strcmp(arg, "sdl"))
    return SDL;
  perror("View type incorrect");
  exit(EXIT_FAILURE);
}

void initView(Controller *controller) {
  switch (controller->view) {
  case NCURSES:
    initViewNcurses(controller);
    break;
  case SDL:
    initViewSdl(controller);
    break;
  }
  controller->model->mainMenu.isOpen = true;
}

void closeView(ViewType viewType) {
  switch (viewType) {
  case NCURSES:
    closeViewNcurses();
    break;
  case SDL:
    closeViewSdl();
    break;
  }
}

Event scanEvent(ViewType viewType) {
  switch (viewType) {
  case NCURSES:
    return scanEventNcurses();
  case SDL:
    return scanEventSdl();
  }
  return NO_EVENT;
}

void createMainMenu(Controller *controller) {
  switch (controller->view) {
  case NCURSES:
    createMainMenuNcurses(controller);
    break;
  case SDL:
    break;
  }
}
void updateMainMenu(Controller *controller) {
  switch (controller->view) {
  case NCURSES:
    updateMainMenuNcurses(controller);
    break;
  case SDL:
    break;
  }
}
void destroyMainMenu(ViewType viewType) {
  switch (viewType) {
  case NCURSES:
    destroyMainMenuNcurses();
    break;
  case SDL:
    break;
  }
}

void createGame(Controller *controller) {
  switch (controller->view) {
  case NCURSES:
    createGameNcurses(controller);
    break;
  case SDL:
    break;
  }
}
void updateGame(Controller *controller) {
  switch (controller->view) {
  case NCURSES:
    updateGameNcurses(controller);
    break;
  case SDL:
    break;
  }
}
void destroyGame(ViewType viewType) {
  switch (viewType) {
  case NCURSES:
    destroyGameNcurses();
    break;
  case SDL:
    break;
  }
}

void resize(Controller *controller) {
  switch (controller->view) {
  case NCURSES:
    resizeNcurses(controller);
    break;
  case SDL:
    break;
  }
}
