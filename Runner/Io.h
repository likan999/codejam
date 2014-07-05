#pragma once

#include <cstddef>
#include <iostream>

namespace io {

template <
  bool StringOccupiesWholeLineEnabled
>
struct ReaderOption {
  static constexpr bool StringOccupiesWholeLine = StringOccupiesWholeLineEnabled;
  template <bool Enabled>
  using SetStringOccupiesWholeLine = ReaderOption<Enabled>;
};

using DefaultReaderOption = ReaderOption<
  /* DoesStringOccupiesWholeLine = */ false
>;

template <typename T, typename Option = DefaultReaderOption, typename Enabled = void>
struct Reader {
  static void read(std::istream& is, T& input);
};

template <typename T, typename Enabled = void>
struct Writer {
  static void write(std::ostream& os, const T& output);
};

// A dummy parameter placed in the function parameter list to inform reader to
// read the following N parameters by scanf.
// As string literal can't be passed to template as non-type parameter, user's
// should use:
// const char s[] = "%d:%f";
// int solver(Scanf<2, s>, int d, float f);
template <std::size_t N, const char* Format>
struct Scanf {
};

// A dummy parameter placed in the function parameter list to inform the reader
// the following N parameters are vectors, and their sizes are read first.
// There are totally M sizes to read, and the i-th vector has the j-th size if
// Indices[i] = j.  If Indices is omitted, then M must be equal to N and each
// vector has its own size in the size list.
template <std::size_t N, std::size_t M = N, std::size_t... Indices>
struct PackedVector {
};

// A dummy parameter placed in the function parameter list to inform reader that
// each of the following std::string parameters take whole lines.
template <bool Enabled = true>
struct StringOccupiesWholeLine {
};

} // namespace io

#include "Io-impl.h"
