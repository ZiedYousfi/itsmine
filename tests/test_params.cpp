#include <iostream>
#include <string>
#include <vector>

#include "../src/params.hpp"

int main() {
  int tests_run = 0;
  int tests_passed = 0;

  auto expect_no_throw_and_size = [&](const std::string& s,
                                      std::size_t expected_size) {
    ++tests_run;
    try {
      Params p(s);
      if (p.getSize() == expected_size) {
        ++tests_passed;
        return;
      }
      std::cerr << "Test failed: expected size " << expected_size << " for '"
                << s << "' (got " << p.getSize() << ")\n";
      std::exit(1);
    } catch (const std::exception& e) {
      std::cerr << "Test failed: unexpected exception for '" << s
                << "': " << e.what() << "\n";
      std::exit(1);
    }
  };

  auto expect_no_throw_and_zero = [&](const std::string& s) {
    ++tests_run;
    try {
      Params p(s);
      if (p.getSize() == 0) {
        ++tests_passed;
        return;
      }
      std::cerr << "Test failed: expected size 0 for '" << s << "' (got "
                << p.getSize() << ")\n";
      std::exit(1);
    } catch (const std::exception& e) {
      std::cerr << "Test failed: unexpected exception for '" << s
                << "': " << e.what() << "\n";
      std::exit(1);
    }
  };

  auto expect_throw = [&](const std::string& s) {
    ++tests_run;
    try {
      Params p(s);
      std::cerr << "Test failed: expected exception for '" << s
                << "' but none thrown\n";
      std::exit(1);
    } catch (const std::invalid_argument&) {
      ++tests_passed;
      return;
    } catch (const std::out_of_range&) {
      ++tests_passed;
      return;
    } catch (const std::exception&) {
      ++tests_passed;
      return;
    }
  };

  expect_no_throw_and_zero("");
  std::vector<std::string> should_throw = {"1",  "1K",  "2M", "3G",
                                           "0K", "123", "K"};
  for (const auto& s : should_throw) {
    expect_throw(s);
  }

  std::vector<std::pair<std::string, std::size_t>> should_pass = {
      {"10BB", 10},     {"1KB", 1024},       {"2KB", 2048},
      {"1MB", 1048576}, {"1GB", 1073741824}, {"1TB", 1099511627776ULL}};
  for (const auto& [s, expected_size] : should_pass) {
    expect_no_throw_and_size(s, expected_size);
  }

  std::cout << "All " << tests_passed << " of " << tests_run
            << " additional tests passed." << std::endl;
  return 0;
}
