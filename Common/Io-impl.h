#pragma once

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include "glog/logging.h"

#include "Common/IntegerSequence.h"
#include "Common/StreamScanf.h"

namespace io {

namespace detail {

template <typename Option, typename T>
void readVectorElements(std::istream& is, std::vector<T>& v, std::size_t size) {
  v.resize(size);
  for (std::size_t i = 0; i < size; i++) {
    Reader<T, Option>::read(is, v[i]);
  }
}

template <typename Tuple, typename Option, std::size_t NextIndex, typename... Elements> struct TupleReader;

template <std::size_t NextIndex, typename Option, typename Tuple, std::size_t... I>
void readTuple(std::istream& is, Tuple& tuple, cxx14::index_sequence<I...>) {
  static_assert(NextIndex + sizeof...(I) == std::tuple_size<Tuple>::value, "Incorrect template parameter");
  TupleReader<Tuple, Option, NextIndex, typename std::tuple_element<NextIndex + I, Tuple>::type...>::read(is, tuple);
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

  template <std::size_t... I>
  static void read(std::istream& is, Tuple& tuple, cxx14::index_sequence<I...>) {
    CHECK(N == io::streamScanf(is, Format, &std::get<NextIndex + 1 + I>(tuple)...));
  }

  static void read(std::istream& is, Tuple& tuple) {
    read(is, tuple, cxx14::make_index_sequence<N>());
    readTuple<NextIndex + 1 + N, Option>(is, tuple, cxx14::make_index_sequence<sizeof...(Elements) - N>());
  }
};

template <typename Tuple, typename Option, std::size_t NextIndex, size_t N, size_t M, size_t... Indices, typename... Elements>
struct TupleReader<Tuple, Option, NextIndex, PackedVector<N, M, Indices...>, Elements...> {
  static_assert(sizeof...(Indices) == 0 || sizeof...(Indices) == N, "Indices list should have exactly N elements if it is not omitted");
  static_assert(sizeof...(Indices) > 0 || N == M, "PackedVector should have the same N and M if Indices list is omitted");
  static_assert(NextIndex + 1 + sizeof...(Elements) == std::tuple_size<Tuple>::value && sizeof...(Elements) >= N, "Incorrect template parameter");

  template <std::size_t... I>
  static void readSizes(std::istream& is, std::size_t sizes[M], cxx14::index_sequence<I...>) {
    int dummy[] = {
      ((void)Reader<std::size_t, Option>::read(is, sizes[I]), 0)...
    };
  }

  template <std::size_t... I>
  static void readVectors(std::istream& is, Tuple& tuple, const std::size_t sizes[N], cxx14::index_sequence<I...>, cxx14::index_sequence<>) {
    int dummy[] = {
      ((void)readVectorElements<Option>(is, std::get<NextIndex + 1 + I>(tuple), sizes[I]), 0)...
    };
  }

  template <std::size_t... I, size_t... J>
  static void readVectors(std::istream& is, Tuple& tuple, const std::size_t sizes[N], cxx14::index_sequence<I...>, cxx14::index_sequence<J...>) {
    int dummy[] = {
      ((void)readVectorElements<Option>(is, std::get<NextIndex + 1 + I>(tuple), sizes[ktl::NthIndex<I, J...>::value]), 0)...
    };
  }

  static void read(std::istream& is, Tuple& tuple) {
    std::size_t sizes[M];
    readSizes(is, sizes, cxx14::make_index_sequence<M>());
    readVectors(is, tuple, sizes, cxx14::make_index_sequence<N>(), cxx14::index_sequence<Indices...>());
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

template <typename Tuple, std::size_t... I>
void writeTuple(std::ostream& os, const Tuple& tuple, cxx14::index_sequence<I...>) {
  int dummy[] = {
    (I == 0 ? (void)0 : (void)(os << ' '), Writer<typename std::tuple_element<I, Tuple>::type>::write(os, std::get<I>(tuple)), 0)...
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
    while (std::getline(is, input) && std::all_of(input.begin(), input.end(), [](char c) { return std::isspace(c); }));
  }
};

// Specialization for vector types.
template <typename T, typename Option>
struct Reader<std::vector<T>, Option, void> {
  static void read(std::istream& is, std::vector<T>& input) {
    std::size_t size;
    Reader<std::size_t, Option>::read(is, size);
    input.resize(size);
    detail::readVectorElements<Option>(is, input, size);
  }
};

// Disable operator>> for dummy parameters.
template <std::size_t N, const char* Format>
std::istream& operator>>(std::istream&, Scanf<N, Format>&) = delete;

template <bool Enabled>
std::istream& operator>>(std::istream&, StringOccupiesWholeLine<Enabled>&) = delete;

} // namespace io
