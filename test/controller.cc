#include "../controller/ds3.h"
#include "../controller/ds4.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>

auto main(int argc, char **argv) -> int {
  try {
    std::shared_ptr<controller::base> ctrl;
    if (argc > 1 && std::string(argv[1]) == "ds4")
      ctrl = std::make_shared<controller::ds4>();
    else
      ctrl = std::make_shared<controller::ds3>();

    std::thread th([&ctrl] { ctrl->run(); });

    std::vector<std::string> keys = {
        "LX",     "LY",       "RX",    "RY", "L2a",  "R2a",    "DX",
        "DY",     "LEFT",     "RIGHT", "UP", "DOWN", "SQUARE", "CROSS",
        "CIRCLE", "TRIANGLE", "L1",    "R1", "L2",   "R2",     "START",
        "SELECT", "L3",       "R3",    "PS", "PAD"};

    for (;;) {
      ctrl->update();

      std::cout << "\e[2;0H";
      for (auto key : keys) {
        std::cout << key << " : " << std::setw(6)
                  << static_cast<int>(ctrl->value(key)) << std::endl;
      }
      usleep(2000);
    }
    th.join();
  } catch (controller::base::error e) {
    std::cerr << e.message << std::endl;
  }
  return 0;
}
