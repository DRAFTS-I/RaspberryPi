#ifndef CONTROLLER_BASE_H
#define CONTROLLER_BASE_H

#include <cstdio>
#include <fcntl.h>
#include <iomanip>
#include <limits>
#include <linux/joystick.h>
#include <map>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

namespace controller {

class base {
public:
  struct error {
    std::string message;
  };
  struct buffer {
    std::map<std::string, int> keys;
  };
  base();
  ~base();
  virtual void run() = 0;
  void update();
  int value(std::string key);

protected:
  std::vector<std::string> keys_;
  const char *JOY_DEV = "/dev/input/js0";
  int joy_fd_ = -1;
  int num_of_axis_ = 0;
  int num_of_buttons_ = 0;
  char num_of_joystick_[80];
  buffer reception;
  buffer stability;
};

base::base() {
  if ((joy_fd_ = open(JOY_DEV, O_RDONLY)) < 0) {
    error e = {"failed to open /dev/input/js0"};
    throw e;
  }
  ioctl(joy_fd_, JSIOCGAXES, &num_of_axis_);
  ioctl(joy_fd_, JSIOCGBUTTONS, &num_of_buttons_);
  ioctl(joy_fd_, JSIOCGNAME(80), &num_of_joystick_);

  for (auto key : keys_) {
    reception.keys[key] = 0;
    stability.keys[key] = 0;
  }
  fcntl(joy_fd_, F_SETFL, O_NONBLOCK);
}

base::~base() { close(joy_fd_); }

void base::update() { stability = reception; };

int base::value(std::string key) {
  try {
    return static_cast<int>(stability.keys[key]);
  } catch (...) {
    return std::numeric_limits<char>::max();
  }
}
} // namespace controller
#endif
