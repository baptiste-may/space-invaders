#pragma once

#include "../model/model.h"

/**
 * @enum Event
 * @brief The event sended by the user every frame
 */
typedef enum {
  NO_EVENT = 0,              //!< No event was sended
  EVENT_KEY_UP = 1,          //!< Key pressed up
  EVENT_KEY_DOWN = 1 << 1,   //!< Key pressed down
  EVENT_KEY_LEFT = 1 << 2,   //!< Key pressed left
  EVENT_KEY_RIGHT = 1 << 3,  //!< Key pressed right
  EVENT_KEY_ENTER = 1 << 4,  //!< Key pressed enter
  EVENT_KEY_ESCAPE = 1 << 5, //!< Key pressed escape
  EVENT_RESIZE = 1 << 6,     //!< The window is resized
  EVENT_CLOSE = 1 << 7,      //!< The window is closed
  EVENT_KEY_SPACE = 1 << 8   //!< Key pressed space
} Event;

/**
 * @struct Controller
 * @brief The structure of the controller
 */
typedef struct {
  Model *model; //!< The model where the informations are stocked
  int view;     //!< The view used for the user
} Controller;

/**
 * @brief The main loop of the controller
 *
 * @param controller The controller used
 */
void mainLoop(Controller *controller);
