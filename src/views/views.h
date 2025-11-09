#ifndef VIEWS_H
#define VIEWS_H

typedef enum { NOT_DEFINED, NCURSES, SDL } ViewType;

ViewType getViewType(const char *arg);

#endif
