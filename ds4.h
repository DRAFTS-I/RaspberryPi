#ifndef DS4_H
#define DS4_H

#include <cstdio>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <linux/joystick.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

// 構造体とかの定義
struct error {
  char message[255];
};
struct ds4form {
  std::vector<int> joy_axis;
  std::vector<bool> joy_button;
};

class ds4 {
public:
  ds4();
  ~ds4();
  void run();
  void update();
  // 鬼のゲッター関数群/*{{{*/
  int LX();
  int LY();
  int RX();
  int RY();
  int L2a();
  int R2a();
  int DX();
  int DY();
  bool D_LEFT();
  bool D_RIGHT();
  bool D_UP();
  bool D_DOWN();
  bool square();
  bool cross();
  bool circle();
  bool triangle();
  bool L1();
  bool R1();
  bool L2d();
  bool R2d();
  bool SHARE();
  bool OPTION();
  bool L3();
  bool R3();
  bool PS();
  bool PAD();
  /*}}}*/
private:
  const char *JOY_DEV = "/dev/input/js0";
  int joy_fd_ = -1;
  int num_of_axis_ = 0;
  int num_of_buttons_ = 0;
  char name_of_joystick_[80];
  ds4form reception;
  ds4form stability;
};

// メンバ関数の動作
// コンストラクタ ファイルが開けなかったら例外投げる/*{{{*/
ds4::ds4() {
  if ((joy_fd_ = open(JOY_DEV, O_RDONLY)) < 0) {
    error e = {"failed to open /dev/input/js0"};
    throw e;
  }
  ioctl(joy_fd_, JSIOCGAXES, &num_of_axis_);
  ioctl(joy_fd_, JSIOCGBUTTONS, &num_of_buttons_);
  ioctl(joy_fd_, JSIOCGNAME(80), &name_of_joystick_);

  reception.joy_button.resize(num_of_buttons_, 0);
  reception.joy_axis.resize(num_of_axis_, 0);
  stability.joy_button.resize(num_of_buttons_, 0);
  stability.joy_axis.resize(num_of_axis_, 0);

  if (reception.joy_axis.size() != 8) {
    error e = {"axis are few"};
    throw e;
  }
  if (reception.joy_button.size() != 14) {
    error e = {"buttons are few"};
    throw e;
  }

  fcntl(joy_fd_, F_SETFL, O_NONBLOCK);
} /*}}}*/
// デストラクタ ファイルを解放する/*{{{*/
ds4::~ds4() { close(joy_fd_); } /*}}}*/
// ボタン入力を受け取る関数/*{{{*/
void ds4::run() {
  while (true) {
    js_event js;
    read(joy_fd_, &js, sizeof(js_event));
    switch (js.type & ~JS_EVENT_INIT) {
    case JS_EVENT_AXIS: {
      reception.joy_axis[static_cast<int>(js.number)] = js.value;
      break;
    }
    case JS_EVENT_BUTTON: {
      reception.joy_button[static_cast<int>(js.number)] =
          static_cast<bool>(js.value);
      break;
    }
    }
    usleep(200);
  }
} /*}}}*/
// ボタンの状態を更新する関数/*{{{*/
void ds4::update() { stability = reception; }
/*}}}*/
// 鬼のゲッター関数群/*{{{*/
int ds4::LX() { return stability.joy_axis[0]; }
int ds4::LY() { return stability.joy_axis[1]; }
int ds4::RX() { return stability.joy_axis[2]; }
int ds4::RY() { return stability.joy_axis[5]; }
int ds4::L2a() { return stability.joy_axis[3]; }
int ds4::R2a() { return stability.joy_axis[4]; }
int ds4::DX() { return stability.joy_axis[6]; }
int ds4::DY() { return stability.joy_axis[7]; }
bool ds4::D_LEFT() { return stability.joy_axis[6] == -32767; }
bool ds4::D_RIGHT() { return stability.joy_axis[6] == 32767; }
bool ds4::D_UP() { return stability.joy_axis[7] == -32767; }
bool ds4::D_DOWN() { return stability.joy_axis[7] == 32767; }
bool ds4::square() { return stability.joy_button[0]; }
bool ds4::cross() { return stability.joy_button[1]; }
bool ds4::circle() { return stability.joy_button[2]; }
bool ds4::triangle() { return stability.joy_button[3]; }
bool ds4::L1() { return stability.joy_button[4]; }
bool ds4::R1() { return stability.joy_button[5]; }
bool ds4::L2d() { return stability.joy_button[6]; }
bool ds4::R2d() { return stability.joy_button[7]; }
bool ds4::SHARE() { return stability.joy_button[8]; }
bool ds4::OPTION() { return stability.joy_button[9]; }
bool ds4::L3() { return stability.joy_button[10]; }
bool ds4::R3() { return stability.joy_button[11]; }
bool ds4::PS() { return stability.joy_button[12]; }
bool ds4::PAD() { return stability.joy_button[13]; }
/*}}}*/
#endif
