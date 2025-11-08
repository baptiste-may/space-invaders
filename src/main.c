#include "model/model.h"
#include <stdlib.h>

int main() {
  Model model = {NULL};
  startGame(&model);

  destroyModel(&model);
  return EXIT_SUCCESS;
}
