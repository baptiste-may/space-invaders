#include "views.h"
#include <string.h>

ViewType getViewType(const char *arg) {
  if (!strcmp(arg, "ncurses"))
    return NCURSES;
  if (!strcmp(arg, "sdl"))
    return SDL;
  return NOT_DEFINED;
}
