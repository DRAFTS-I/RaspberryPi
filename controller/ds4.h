#ifndef CONTROLLER_DS4_H
#define CONTROLLER_DS4_H

#include "base.h"

namespace controller {

class ds4 : public base {
public:
  ds4();
  void run() override;
};

ds4::ds4() : base() {
  keys_ = {"LX",  "LY",     "RX",    "L2a",    "R2a",      "RY", "DX",
           "DY",  "SQUARE", "CROSS", "CIRCLE", "TRIANGLE", "L1", "R1",
           "L2",  "R2",     "START", "SELECT", "L3",       "R3", "PS",
           "PAD", "LEFT",   "RIGHT", "UP",     "DOWN"};
}

void ds4::run() {
  for (;;) {
    js_event js;
    read(joy_fd_, &js, sizeof(js_event));
    switch (js.type & ~JS_EVENT_INIT) {
    case JS_EVENT_AXIS: {
      if (static_cast<int>(js.number) < num_of_axis_) {
        reception.keys[keys_[static_cast<int>(js.number)]] =
            static_cast<int>(js.value >> 8);
      }
      reception.keys["LEFT"] = reception.keys["DX"] < -64;
      reception.keys["RIGHT"] = reception.keys["DX"] > 64;
      reception.keys["UP"] = reception.keys["DY"] < -64;
      reception.keys["DOWN"] = reception.keys["DY"] > 64;

      break;
    }
    case JS_EVENT_BUTTON: {
      if (static_cast<int>(js.number) < 17) {
        reception.keys[keys_[static_cast<int>(js.number) + num_of_axis_]] =
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
