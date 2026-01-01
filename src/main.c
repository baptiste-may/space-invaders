#include "controller/controller.h"
#include "model/model.h"
#include "views/views.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **args) {
  if (argc < 2) {
    printf("Not enough arguments\n");
    return EXIT_FAILURE;
  }

  ViewType viewType = getViewType(args[1]);
  Model model = newModel();
  Controller *controller = initView(viewType, &model);

  mainLoop(controller);

  controller->view->close();
  free(controller->view);
  free(controller);
  destroyModel(&model);
  return EXIT_SUCCESS;
}
