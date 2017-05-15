#ifndef DS3_H
#define DS3_H

#include <cstdio>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <linux/joystick.h>
#include <map>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

// 構造体とかの定義
struct error {
  std::string message;
};
struct ds3form {
  std::map<std::string, int> keys;
};

class ds3 {
public:
  ds3();
  ~ds3();
  void run();
  void update();
  int getKey(std::string key);

private:
  std::vector<std::string> keys_ = {
      "LX",      "LY",  "RX",       "RY",     "YAW",       "PITCH",   "ROLL",
      "?",       "UP",  "RIGHT",    "DOWN",   "LEFT",      "L2",      "R2",
      "L1",      "R1",  "TRIANGLE", "CIRCLE", "CROSS",     "SQUARE",  "SELECT",
      "L3",      "R3",  "START",    "UPd",    "RIGHTd",    "DOWNd",   "LEFTd",
      "L2d",     "R2d", "L1d",      "L2d",    "TRIANGLEd", "CIRCLEd", "CROSSd",
      "SQUAREd", "PS"};
  const char *JOY_DEV = "/dev/input/js0";
  int joy_fd_ = -1;
  int num_of_axis_ = 0;
  int num_of_buttons_ = 0;
  char name_of_joystick_[80];
  ds3form reception;
  ds3form stability;
};

// メンバ関数の動作
// コンストラクタ ファイルが開けなかったら例外投げる/*{{{*/
ds3::ds3() {
  if ((joy_fd_ = open(JOY_DEV, O_RDONLY)) < 0) {
    error e = {"failed to open /dev/input/js0"};
    throw e;
  }
  ioctl(joy_fd_, JSIOCGAXES, &num_of_axis_);
  ioctl(joy_fd_, JSIOCGBUTTONS, &num_of_buttons_);
  ioctl(joy_fd_, JSIOCGNAME(80), &name_of_joystick_);

  // マップをそれぞれのキーで初期化
  for (auto key : keys_) {
    reception.keys[key] = 0;
    stability.keys[key] = 0;
  }
  fcntl(joy_fd_, F_SETFL, O_NONBLOCK);
} /*}}}*/

// デストラクタ ファイルを解放する/*{{{*/
ds3::~ds3() { close(joy_fd_); } /*}}}*/

// ボタン入力を受け取る関数/*{{{*/
void ds3::run() {
  while (true) {
    js_event js;
    read(joy_fd_, &js, sizeof(js_event));
    switch (js.type & ~JS_EVENT_INIT) {
    case JS_EVENT_AXIS: {
      if (static_cast<int>(js.number) < 20) {
        reception.keys[keys_[static_cast<int>(js.number)]] =
            static_cast<int>(js.value);
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
} /*}}}*/

// ボタンの状態を更新する関数/*{{{*/
void ds3::update() { stability = reception; }
/*}}}*/

// キーを取得する関数/*{{{*/
int ds3::getKey(std::string key) { return stability.keys[key]; } /*}}}*/
#endif
