#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

#include "params.hpp"

int main(int argc, char** argv) {
  std::cout << "Hello, from itsmine!\n";

  (void)argc;
  (void)argv;

  AllocationParams::Params* params = nullptr;

  try {
    params = new AllocationParams::Params("1MB");
    std::cout << "Params object created successfully.\n";
    std::cout << "Parsed size: " << params->getSize() << " "
              << params->getBytesTypeString() << "\n";
  } catch (const std::invalid_argument& e) {
    std::cerr << "Failed to create Params object: " << e.what() << "\n";
    return 1;
  }

  void* ptr = malloc(params->getSize());

  if (ptr == nullptr) {
    std::cerr << "Memory allocation failed.\n";
    delete params;
    return 1;
  } else {
    std::cout << "Memory allocated successfully: " << params->getSize()
              << " bytes.\n";

    // Just a tiny dummy operation on the first few bytes to avoid
    // performing an impractically long loop when large allocations are used.
    const std::size_t work = static_cast<std::size_t>(params->getSize());
    for (std::size_t i = 0; i < work; ++i) {
      static_cast<char*>(ptr)[i] <<= 1;
      std::cout << "Processing byte " << (i + 1) << "/" << work << "\n";
    }

    std::cout << "Dummy operation completed.\n";
    free(ptr);
    ptr = nullptr;
    std::cout << "Memory freed successfully.\n";
  }

  return 0;
}
