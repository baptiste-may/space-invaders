#ifndef SDL_VIEW_H
#define SDL_VIEW_H

#include "../model/model.h"

#define WIDTH 600
#define HEIGHT 400

/**
 * @brief Initialize view using sdl
 */
void initViewSdl();

/**
 * @brief Close view using sdl
 */
void closeViewSdl();

/**
 * @brief Loop view using sdl
 */
void loopViewSdl(Model *model);

#endif
