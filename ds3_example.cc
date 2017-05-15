#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "ds3.h"

int main() {
  try {
    ds3 controller;
    std::thread th([&controller] { controller.run(); });
    while (true) {
      // データベースの更新
      controller.update();

      // いい感じに表示するためのおまじない 詳しくはエスケープシーケンスで
      std::cout << "\e[2;0H";
      // 値の取り方のサンプルコード
      std::cout << controller.getKey("LX") << std::endl; // こうやってとる

      // 動作確認用に全部の値を出力{{{
      std::vector<std::string> keys_ = {
          "LX",    "LY",     "RX",        "RY",      "YAW",      "PITCH",
          "ROLL",  "?",      "UP",        "RIGHT",   "DOWN",     "LEFT",
          "L2",    "R2",     "L1",        "R1",      "TRIANGLE", "CIRCLE",
          "CROSS", "SQUARE", "SELECT",    "L3",      "R3",       "START",
          "UPd",   "RIGHTd", "DOWNd",     "LEFTd",   "L2d",      "R2d",
          "L1d",   "L2d",    "TRIANGLEd", "CIRCLEd", "CROSSd",   "SQUAREd",
          "PS"};
      for (auto key : keys_) {
        std::cout << key << ": " << std::setw(6) << controller.getKey(key)
                  << std::endl;
      } /*}}}*/

      // 一定時間待機
      usleep(2000);
    }
    th.join();
  } catch (error &e) {
    std::cerr << e.message << std::endl;
  }
  return 0;
}
