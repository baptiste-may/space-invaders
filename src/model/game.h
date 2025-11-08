#ifndef GAME_H
#define GAME_H

#define DEFAULT_LIVES 3

typedef struct {
  unsigned score, lives, nbAliens, nbAlienRows, nbBuildings;
  char *aliens;
} Game;

Game *createGame(unsigned nbAliens, unsigned nbAlienRows, unsigned nbBuildings);
void freeGame(Game *game);

#endif
