#pragma once

#define SHIELD_WIDTH 11
#define SHIELD_HEIGHT 6

typedef enum _ShieldBlock {
  EMPTY,
  FULL,
  HALF_FULL_LEFT,
  HALF_FULL_RIGHT,
  HALF_EMPTY_LEFT,
  HALF_EMPTY_RIGHT,
  DESTROYED_LEFT,
  DESTROYED_RIGHT
} ShieldBlock;

typedef ShieldBlock Shield[SHIELD_HEIGHT][SHIELD_WIDTH];

typedef struct _Shields {
  unsigned nb;
  Shield *blocks;
} Shields;

Shield *createShields(unsigned nbShields);
void freeShields(Shields *shields);
