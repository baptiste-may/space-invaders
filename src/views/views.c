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

Controller *initView(ViewType viewType, Model *model) {
  Controller *controller = malloc(sizeof(Controller));
  if (controller == NULL) {
      perror("Failed to allocate memory for controller");
      exit(EXIT_FAILURE);
  }
  controller->model = model;

  controller->view = malloc(sizeof(ViewInterface));
  if (controller->view == NULL) {
      perror("Failed to allocate memory for view interface");
      free(controller);
      exit(EXIT_FAILURE);
  }

  switch (viewType) {
  case NCURSES:
    *controller->view = getNcursesInterface();
    break;
  case SDL:
    *controller->view = getSdlInterface();
    break;
  }
  
  if (controller->view->init) {
    controller->view->init(controller);
  }
  
  controller->model->mainMenu.isOpen = true;
  return controller;
}