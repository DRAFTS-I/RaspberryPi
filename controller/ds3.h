#ifndef CONTROLLER_DS3_H
#define CONTROLLER_DS3_H

#include "base.h"

namespace controller {

class ds3 : public base {
public:
  ds3();
  void run() override;
};

ds3::ds3() : base() {
  keys_ = {"LX",     "LY",      "RX",       "RY",     "YAW",       "PITCH",
           "ROLL",   "?",       "UPa",      "RIGHTa", "DOWNa",     "LEFTa",
           "L2a",    "R2a",     "L1a",      "R1a",    "TRIANGLEa", "CIRCLEa",
           "CROSSa", "SQUAREa", "SELECT",   "L3",     "R3",        "START",
           "UP",     "RIGHT",   "DOWN",     "LEFT",   "L2",        "R2",
           "L1",     "R1",      "TRIANGLE", "CIRCLE", "CROSS",     "SQUARE",
           "PS"};
}

void ds3::run() {
  for (;;) {
    js_event js;
    read(joy_fd_, &js, sizeof(js_event));
    switch (js.type & ~JS_EVENT_INIT) {
    case JS_EVENT_AXIS: {
      if (static_cast<int>(js.number) < 20) {
        reception.keys[keys_[static_cast<int>(js.number)]] =
            static_cast<int>(js.value >> 8);
      }
      break;
    }
    case JS_EVENT_BUTTON: {
      if (static_cast<int>(js.number) < 17) {
        reception.keys[keys_[static_cast<int>(js.number) + 20]] =
            static_cast<int>(js.value);
      }
      break;
    }
    }
    usleep(200);
  }
}
} // namespace controller
#endif
