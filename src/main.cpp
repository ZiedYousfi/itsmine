#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

#include "params.hpp"

int main(int argc, char** argv) {
  std::cout << "Hello, from itsmine!\n";

  (void)argc;
  (void)argv;

  Params params("1024MB");
  std::cout << "Parsed size: " << params.getSize() << " " << params.getBytesType() << "\n";

  return 0;
}
