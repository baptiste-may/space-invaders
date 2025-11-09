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
  if (viewType == NOT_DEFINED) {
    printf("View incorrect\n");
    return EXIT_FAILURE;
  }

  Model model = {NULL};
  initView(viewType);

  loopView(viewType, &model);

  closeView(viewType);
  destroyModel(&model);
  return EXIT_SUCCESS;
}
