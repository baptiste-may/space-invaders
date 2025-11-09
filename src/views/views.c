#include "views.h"
#include "ncurses-view.h"
#include "sdl-view.h"
#include <string.h>

ViewType getViewType(const char *arg) {
  if (!strcmp(arg, "ncurses"))
    return NCURSES;
  if (!strcmp(arg, "sdl"))
    return SDL;
  return NOT_DEFINED;
}

void initView(ViewType viewType) {
  switch (viewType) {
  case NCURSES:
    initViewNcurses();
    break;
  case SDL:
    initViewSdl();
    break;
  default:
    break;
  }
}

void closeView(ViewType viewType) {
  switch (viewType) {
  case NCURSES:
    closeViewNcurses();
    break;
  case SDL:
    closeViewSdl();
    break;
  default:
    break;
  }
}

void loopView(ViewType viewType, Model *model) {
  switch (viewType) {
  case NCURSES:
    loopViewNcurses(model);
    break;
  case SDL:
    loopViewSdl(model);
    break;
  default:
    break;
  }
}
