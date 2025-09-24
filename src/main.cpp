#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

#include "params.hpp"

int main(int argc, char** argv) {
  std::cout << "Hello, from itsmine!\n";

  (void)argc;
  (void)argv;

  try {
    Params params("1024MB");
    std::cout << "Params object created successfully.\n";
    std::cout << "Parsed size: " << params.getSize() << " "
              << params.getBytesType() << "\n";
  } catch (const std::invalid_argument& e) {
    std::cerr << "Failed to create Params object: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
