#pragma once

#include <iostream>
#include <string>

namespace io {

template <typename T, typename Enabled = void>
struct Reader {
  static void read(std::istream& is, T& input);
};

template <typename T, typename Enabled = void>
struct Writer {
  static void write(std::ostream& os, const T& output);
};

// Reader for strings that occupy the whole line.
// Usually you just make Reader<string> inherit it:
// template <> struct Reader<std::string, void> : WholeLineStringReader {};
struct WholeLineStringReader {
  static void read(std::istream& is, std::string& input) {
    while (std::getline(is, input) && input.empty());
  }
};

} // namespace io

#include "Io-impl.h"
