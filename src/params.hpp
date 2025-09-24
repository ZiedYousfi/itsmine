#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

enum BytesType { Byte, KiloByte, MegaByte, GigaByte, TeraByte };

class Params {
 public:
  Params(std::string argsCollected);
  ~Params() = default;

  std::size_t getSize() const;
  BytesType getBytesType() const;

 private:
  std::size_t size_{};
  BytesType bytesType_{Byte};
};
