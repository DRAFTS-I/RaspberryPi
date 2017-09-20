#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <stdlib.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
#include <wiringPi.h>
#include <wiringPiSPI.h>

constexpr int channel = 0;
constexpr int speed = 1000000;
constexpr int SS = 22;

auto main(int argc, char **argv) -> int {
  wiringPiSetup();
  wiringPiSPISetupMode(channel, speed, 1);
  wiringPiSetupGpio();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, 1);

  for (;;) {
    std::string val;
    std::cout << "input value" << std::endl;
    std::cin >> val;
    int num = std::atoi(val.c_str());
    if (num < 0 || num > 180) {
      continue;
    }

    std::vector<unsigned char> analog_data;
    analog_data.push_back(num);

    digitalWrite(SS, 0);

    for (auto data : analog_data) {
      unsigned char buff;
      buff = data;
      wiringPiSPIDataRW(channel, &buff, 1);
      std::cout << "received value\n" << static_cast<int>(buff) << std::endl;
    }

    // 一定時間待機
    digitalWrite(SS, 1);
    usleep(1000000);
  }
  return 0;
}
