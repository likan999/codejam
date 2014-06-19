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
// read the following parameters by scanf.
// As string literal can't be passed to template as non-type parameter, user's
// should use:
// const char s[] = "%d:%f";
// int solver(Scanf<2, s>, int d, float f);
template <std::size_t N, const char* Format>
struct Scanf {
  static constexpr const char* format = Format;
};

// A dummy parameter placed in the function parameter list to inform reader that
// each of the following std::string parameters take whole lines.
template <bool Enabled = true>
struct StringOccupiesWholeLine {
  static constexpr bool value = Enabled;
};

} // namespace io

#include "Io-impl.h"
