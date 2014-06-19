#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include <glog/logging.h>

#include "Common/IntegerSequence.h"
#include "Common/StreamScanf.h"

namespace io {

namespace detail {

template <typename Tuple, typename Option, std::size_t NextIndex, typename... Elements> struct TupleReader;

template <std::size_t NextIndex, typename Option, typename Tuple, std::size_t... Indices>
void readTuple(std::istream& is, Tuple& tuple, cxx14::index_sequence<Indices...>) {
  static_assert(NextIndex + sizeof...(Indices) == std::tuple_size<Tuple>::value, "Incorrect template parameter");
  TupleReader<Tuple, Option, NextIndex, typename std::tuple_element<NextIndex + Indices, Tuple>::type...>::read(is, tuple);
}

template <typename Tuple, typename Option, std::size_t NextIndex>
struct TupleReader<Tuple, Option, NextIndex> {
  static_assert(NextIndex == std::tuple_size<Tuple>::value, "Incorrect template parameter");

  static void read(std::istream& /*is*/, Tuple& /*tuple*/) {
    // Doing nothing here.
  }
};

template <typename Tuple, typename Option, std::size_t NextIndex, typename NextType, typename... Elements>
struct TupleReader<Tuple, Option, NextIndex, NextType, Elements...> {
  static_assert(NextIndex + 1 + sizeof...(Elements) == std::tuple_size<Tuple>::value, "Incorrect template parameter");

  static void read(std::istream& is, Tuple& tuple) {
    Reader<NextType, Option>::read(is, std::get<NextIndex>(tuple));
    TupleReader<Tuple, Option, NextIndex + 1, Elements...>::read(is, tuple);
  }
};

// Handle Scanf dummy parameter.
template <typename Tuple, typename Option, std::size_t NextIndex, std::size_t N, const char* Format, typename... Elements>
struct TupleReader<Tuple, Option, NextIndex, Scanf<N, Format>, Elements...> {
  static_assert(NextIndex + 1 + sizeof...(Elements) == std::tuple_size<Tuple>::value && sizeof...(Elements) >= N, "Incorrect template parameter");

  template <std::size_t... Indices>
  static void read(std::istream& is, Tuple& tuple, cxx14::index_sequence<Indices...>) {
    CHECK(N == io::streamScanf(is, Format, &std::get<NextIndex + 1 + Indices>(tuple)...));
  }

  static void read(std::istream& is, Tuple& tuple) {
    read(is, tuple, cxx14::make_index_sequence<N>());
    readTuple<NextIndex + 1 + N, Option>(is, tuple, cxx14::make_index_sequence<sizeof...(Elements) - N>());
  }
};

template <typename Tuple, typename Option, std::size_t NextIndex, bool Enabled, typename... Elements>
struct TupleReader<Tuple, Option, NextIndex, StringOccupiesWholeLine<Enabled>, Elements...> {
  static_assert(NextIndex + 1 + sizeof...(Elements) == std::tuple_size<Tuple>::value, "Incorrect template parameter");

  static void read(std::istream& is, Tuple& tuple) {
    using NewOption = typename Option::template SetStringOccupiesWholeLine<Enabled>;
    TupleReader<Tuple, NewOption, NextIndex + 1, Elements...>::read(is, tuple);
  }
};

template <typename Tuple, std::size_t... Indices>
void writeTuple(std::ostream& os, const Tuple& tuple, cxx14::index_sequence<Indices...>) {
  int dummy[] = {
    (Indices == 0 ? (void)0 : (void)(os << ' '), Writer<typename std::tuple_element<Indices, Tuple>::type>::write(os, std::get<Indices>(tuple)), 0)...
  };
}

} // namespace detail

// General implementation for Reader and Writer.
template <typename T, typename Option, typename Enabled>
void Reader<T, Option, Enabled>::read(std::istream& is, T& input) {
  is >> input;
}

template <typename T, typename Enabled>
void Writer<T, Enabled>::write(std::ostream& os, const T& output) {
  os << output;
}

// Specialization for tuple types such as std::tuple, std::pair, std::array, etc.
template <typename Tuple, typename Option>
struct Reader<Tuple, Option, typename std::enable_if<(std::tuple_size<Tuple>::value >= 0)>::type> {
  static void read(std::istream& is, Tuple& tuple) {
    detail::readTuple<0, Option>(is, tuple, cxx14::make_index_sequence<std::tuple_size<Tuple>::value>());
  }
};

template <typename Tuple>
struct Writer<Tuple, typename std::enable_if<(std::tuple_size<Tuple>::value >= 0)>::type> {
  static void write(std::ostream& os, const Tuple& tuple) {
    detail::writeTuple(os, tuple, cxx14::make_index_sequence<std::tuple_size<Tuple>::value>());
  }
};

// Specialization for string types that have StringOccupiesWholeLine enabled.
template <typename Option>
struct Reader<std::string, Option, typename std::enable_if<Option::StringOccupiesWholeLine>::type> {
  static void read(std::istream& is, std::string& input) {
    while (std::getline(is, input) && input.empty());
  }
};

// Specialization for vector types.
template <typename T, typename Option>
struct Reader<std::vector<T>, Option, void> {
  static void read(std::istream& is, std::vector<T>& input) {
    std::size_t size;
    Reader<std::size_t, Option>::read(is, size);
    input.resize(size);
    for (std::size_t i = 0; i < size; i++) {
      Reader<T, Option>::read(is, input[i]);
    }
  }
};

// Disable operator>> for dummy parameters.
template <std::size_t N, const char* Format>
std::istream& operator>>(std::istream&, Scanf<N, Format>&) = delete;

template <bool Enabled>
std::istream& operator>>(std::istream&, StringOccupiesWholeLine<Enabled>&) = delete;

} // namespace io
