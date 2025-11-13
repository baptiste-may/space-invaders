#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/model.h"

/**
 * @enum Event
 * @brief The event sended by the user every frame
 */
typedef enum {
  NO_EVENT,         //!< No event was sended
  EVENT_KEY_UP,     //!< Key pressed up
  EVENT_KEY_DOWN,   //!< Key pressed down
  EVENT_KEY_ENTER,  //!< Key pressed enter
  EVENT_KEY_ESCAPE, //!< Key pressed escape
  EVENT_RESIZE,     //!< The window is resized
  EVENT_CLOSE       //!< The window is closed
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

#endif
