#include "score.h"
#include <stdio.h>
#include <stdlib.h>

unsigned getBestScore() {
  FILE *file = fopen(BEST_SCORE_FILE, "r");
  if (!file)
    return 0;

  char c;
  unsigned res = 0;
  while ((c = fgetc(file)) != EOF) {
    if (c >= '0' && c <= '9') { 
      res *= 10;
      res += (c - '0');
    }
  }
  fclose(file);
  return res;
}

void updateBestScore(Scores *scores) {
  if (scores->current > scores->best) {
    scores->best = scores->current;
    FILE *file = fopen(BEST_SCORE_FILE, "w");
    if (!file)
      return;
    fprintf(file, "%u", scores->best);
    fclose(file);
  }
}
