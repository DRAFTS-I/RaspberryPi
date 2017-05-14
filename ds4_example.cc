#include <iostream>
#include <thread>

#include "ds4.h"

int main() {
  try {
    ds4 controller;
    std::thread th([&controller] { controller.run(); });
    while (true) {
      // データベースの更新
      controller.update();

      // いい感じに表示するためのおまじない 詳しくはエスケープシーケンスで
      std::cout << "\e[2;0H";
      // それぞれの値を出力 値の取り方のサンプルコード
      std::cout << "LX:  " << std::setw(6) << controller.LX() << std::endl;
      std::cout << "LY:  " << std::setw(6) << controller.LY() << std::endl;
      std::cout << "RX:  " << std::setw(6) << controller.RX() << std::endl;
      std::cout << "RY:  " << std::setw(6) << controller.RY() << std::endl;
      std::cout << "L2a: " << std::setw(6) << controller.L2a() << std::endl;
      std::cout << "R2a: " << std::setw(6) << controller.R2a() << std::endl;
      std::cout << "DX:  " << std::setw(6) << controller.DX() << std::endl;
      std::cout << "DY:  " << std::setw(6) << controller.DY() << std::endl;
      std::cout << "D_LEFT:   " << controller.D_LEFT() << std::endl;
      std::cout << "D_RIGHT:  " << controller.D_RIGHT() << std::endl;
      std::cout << "D_UP:     " << controller.D_UP() << std::endl;
      std::cout << "D_DOWN:   " << controller.D_DOWN() << std::endl;
      std::cout << "square:   " << controller.square() << std::endl;
      std::cout << "cross:    " << controller.cross() << std::endl;
      std::cout << "circle:   " << controller.circle() << std::endl;
      std::cout << "triangle: " << controller.triangle() << std::endl;
      std::cout << "L1:       " << controller.L1() << std::endl;
      std::cout << "R1:       " << controller.R1() << std::endl;
      std::cout << "L2d:      " << controller.L2d() << std::endl;
      std::cout << "R2d:      " << controller.R2d() << std::endl;
      std::cout << "SHARE:    " << controller.SHARE() << std::endl;
      std::cout << "OPTION:   " << controller.OPTION() << std::endl;
      std::cout << "L3:       " << controller.L3() << std::endl;
      std::cout << "R3:       " << controller.R3() << std::endl;
      std::cout << "PS:       " << controller.PS() << std::endl;
      std::cout << "PAD:      " << controller.PAD() << std::endl;

      // 一定時間待機
      usleep(2000);
    }
    th.join();
  } catch (error &e) {
    std::cerr << e.message << std::endl;
  }
  return 0;
}
