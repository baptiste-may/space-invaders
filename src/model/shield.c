#include "shield.h"
#include <stdio.h>
#include <stdlib.h>

const ShieldBlock shieldTemplate[SHIELD_HEIGHT][SHIELD_WIDTH] = {
    {EMPTY, HALF_EMPTY_RIGHT, FULL, FULL, FULL, FULL, FULL, FULL, FULL,
     HALF_EMPTY_LEFT, EMPTY},
    {HALF_EMPTY_RIGHT, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL,
     HALF_EMPTY_LEFT},
    {FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL},
    {FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL},
    {FULL, FULL, HALF_FULL_RIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
     HALF_FULL_LEFT, FULL, FULL}};

Shield *createShields(unsigned nbShields) {
  Shield *res = (Shield *)malloc(sizeof(Shield) * nbShields);
  if (!res) {
    perror("Alllocation error");
    exit(EXIT_FAILURE);
  }

  for (unsigned n = 0; n < nbShields; n++) {
    for (unsigned i = 0; i < SHIELD_HEIGHT; i++) {
      for (unsigned j = 0; j < SHIELD_WIDTH; j++) {
        res[n][i][j] = shieldTemplate[i][j];
      }
    }
  }

  return res;
}

void freeShields(Shields *shields) {
  if (shields && shields->blocks)
    free(shields->blocks);
}
