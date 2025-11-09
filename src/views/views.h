#ifndef VIEWS_H
#define VIEWS_H

/**
 * @enum ViewType
 * @breif The type of view used to play the game
 *
 * @var ViewType NOT_DEFINED
 * @breif The view is not defined, the game cannot be launched
 *
 * @var ViewType NCURSES
 * @breif The app use Ncurses for the view
 *
 * @var ViewType SDL
 * @breif The app use SDL2 for the view
 */
typedef enum { NOT_DEFINED, NCURSES, SDL } ViewType;

/**
 * @breif Return the correct enum depending on the string argument
 *
 * @param arg The string argument
 * @return The enum associated
 */
ViewType getViewType(const char *arg);

#endif
