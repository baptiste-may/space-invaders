#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/model.h"

typedef enum {
  NO_EVENT,
  EVENT_KEY_UP,
  EVENT_KEY_DOWN,
  EVENT_KEY_ENTER,
  EVENT_KEY_ESCAPE,
  EVENT_RESIZE,
  EVENT_CLOSE
} Event;

typedef struct {
  Model *model;
  int view;
} Controller;

void mainLoop(Controller *controller);

#endif
