#include "params.hpp"

Params::Params(std::string argsCollected) {
  {
    if (argsCollected.empty()) {
      size_ = 0;
      bytesType_ = Byte;
      return;
    }

    char lastCharLocal = argsCollected.back();
    char secondLastChar = '\0';
    bool hasSecondLast = argsCollected.size() >= 2;
    if (hasSecondLast) {
      secondLastChar = argsCollected[argsCollected.size() - 2];
    }

    bool lastIsDigit = (lastCharLocal >= '0' && lastCharLocal <= '9');
    bool secondLastIsDigit =
        hasSecondLast && (secondLastChar >= '0' && secondLastChar <= '9');

    if (lastIsDigit || secondLastIsDigit) {
      throw std::invalid_argument("Invalid size format: no unit specified");
      return;
    }

    BytesType bt = Byte;

    if (!argsCollected.empty()) {
      // Look at the second-last character (commonly the unit letter
      // when a trailing 'B' is present). Use uppercase for case-insenstive
      // matching.
      char secondLast = argsCollected.size() >= 2
                            ? static_cast<char>(std::toupper(
                                  argsCollected[argsCollected.size() - 2]))
                            : '\0';
      switch (secondLast) {
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

    std::string numberStr = argsCollected;
    // If we detected a unit (e.g. 'M' + 'B'), strip the final two characters
    // so the numeric portion is the pure digits only. For other inputs
    // (like "10BB") leave the string intact and rely on stoull to parse
    // the initial numeric part.
    if (bt != Byte && numberStr.size() >= 2) {
      numberStr = numberStr.substr(0, numberStr.size() - 2);
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
    bytesType_ = bt;
  }
}

std::size_t Params::getSize() const { return size_; }
BytesType Params::getBytesType() const { return bytesType_; }

std::string Params::getBytesTypeString() const {
  switch (bytesType_) {
    case Byte:
      return "B";
    case KiloByte:
      return "KB";
    case MegaByte:
      return "MB";
    case GigaByte:
      return "GB";
    case TeraByte:
      return "TB";
    default:
      return "B";
  }
}
