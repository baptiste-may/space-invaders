#include "ncurses-view.h"
#include <ncurses.h>

void initViewNcurses() { initscr(); }

void closeViewNcurses() { endwin(); }

void loopViewNcurses(Model *model) {
  refresh();
  getch();
}
