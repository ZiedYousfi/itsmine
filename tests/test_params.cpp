// Minimal, dependency-free test runner for Params
#include <iostream>
#include <string>

#include "../src/params.hpp"

static int tests_run = 0;

inline void expect_eq(std::size_t a, std::size_t b, const std::string& msg) {
  ++tests_run;
  if (a != b) {
    std::cerr << "Test failed: " << msg << " (" << a << " != " << b << ")\n";
    std::exit(1);
  }
}

int main() {
  // Happy paths
  expect_eq(Params("1").getSize(), 1ULL, "plain number");
  expect_eq(Params("1K").getSize(), 1024ULL, "1K -> 1024");
  expect_eq(Params("2M").getSize(), 2ULL * 1024ULL * 1024ULL,
            "2M -> 2*1024*1024");
  expect_eq(Params("3G").getSize(), 3ULL * 1024ULL * 1024ULL * 1024ULL,
            "3G -> 3*1024^3");

  // Edge cases
  expect_eq(Params("").getSize(), 0ULL, "empty -> 0");
  expect_eq(Params("0K").getSize(), 0ULL, "0K -> 0");

  std::cout << "All " << tests_run << " tests passed.\n";
  return 0;
}
