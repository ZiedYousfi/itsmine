#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

#include "params.hpp"

int main(int argc, char** argv) {
  std::cout << "Hello, from itsmine!\n";

  (void)argc;
  (void)argv;

  Params params("1024KB");
  std::cout << "Parsed size: " << params.getSize() << " bytes\n";

  return 0;
}
