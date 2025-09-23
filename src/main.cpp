#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

enum BytesType { Byte, KiloByte, MegaByte, GigaByte, TeraByte };

class Params {
 public:
  Params(std::string argsCollected) {
    BytesType bt = Byte;

    if (!argsCollected.empty()) {
      char lastChar = argsCollected.at((std::size_t)(argsCollected.size() - 1));
      switch (lastChar) {
        case 'K':
          bt = KiloByte;
          break;
        case 'M':
          bt = MegaByte;
          break;
        case 'G':
          bt = GigaByte;
          break;
        case 'T':
          bt = TeraByte;
          break;
        default:
          bt = Byte;
          break;
      }
    }

    // Extract numeric part (remove unit suffix if present)
    std::string numberStr = argsCollected;
    if (bt != Byte && !numberStr.empty()) {
      numberStr.pop_back();
    }

    std::size_t charactersAfterNumber{0};
    std::uint64_t demandedSize = 0;
    if (!numberStr.empty()) {
      demandedSize = std::stoull(numberStr, &charactersAfterNumber, 10);
    }

    std::uint64_t multiplier = 1;
    switch (bt) {
      case KiloByte:
        multiplier = 1024ULL;
        break;
      case MegaByte:
        multiplier = 1024ULL * 1024ULL;
        break;
      case GigaByte:
        multiplier = 1024ULL * 1024ULL * 1024ULL;
        break;
      case TeraByte:
        multiplier = 1024ULL * 1024ULL * 1024ULL * 1024ULL;
        break;
      default:
        multiplier = 1ULL;
        break;
    }

    size_ = static_cast<std::size_t>(demandedSize * multiplier);
  }
  ~Params() {}

  std::size_t getSize() const { return size_; }

 private:
  std::size_t size_;
};

int main(int argc, char** argv) {
  std::cout << "Hello, from itsmine!\n";

  (void)argc;
  (void)argv;

  return 0;
}
