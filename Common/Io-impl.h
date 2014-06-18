#pragma once

#include <iostream>
#include <tuple>
#include <type_traits>
#include <vector>

namespace io {

namespace {

template <size_t Next, typename Tuple, typename Enabled = void> struct TupleReader;

template <size_t Next, typename Tuple>
struct TupleReader<Next, Tuple, typename std::enable_if<Next < std::tuple_size<Tuple>::value>::type> {
  static void read(std::istream& is, Tuple& input) {
    Reader<typename std::tuple_element<Next, Tuple>::type>::read(is, std::get<Next>(input));
    TupleReader<Next + 1, Tuple>::read(is, input);
  }
};

template <size_t Next, typename Tuple>
struct TupleReader<Next, Tuple, typename std::enable_if<Next == std::tuple_size<Tuple>::value>::type> {
  static void read(std::istream& /*is*/, Tuple& /*input*/) {
  }
};

template <size_t Next, typename Tuple, typename Enabled = void> struct TupleWriter;

template <size_t Next, typename Tuple>
struct TupleWriter<Next, Tuple, typename std::enable_if<Next < std::tuple_size<Tuple>::value>::type> {
  static void write(std::ostream& os, Tuple& output) {
    if (Next > 0) {
      os << ' ';
    }
    Writer<typename std::tuple_element<Next, Tuple>::type>::write(os, std::get<Next>(output));
    TupleWriter<Next + 1, Tuple>::write(os, output);
  }
};

template <size_t Next, typename Tuple>
struct TupleWriter<Next, Tuple, typename std::enable_if<Next == std::tuple_size<Tuple>::value>::type> {
  static void write(std::ostream& /*os*/, Tuple& /*output*/) {
  }
};

} // namespace

// General implementation for Reader and Writer.
template <typename T, typename Enabled>
void Reader<T, Enabled>::read(std::istream& is, T& input) {
  is >> input;
}

template <typename T, typename Enabled>
void Writer<T, Enabled>::write(std::ostream& os, const T& output) {
  os << output;
}

// Specialization for tuple types such as std::tuple, std::pair, std::array, etc.
template <typename Tuple>
struct Reader<Tuple, typename std::enable_if<(std::tuple_size<Tuple>::value > 0)>::type> {
  static void read(std::istream& is, Tuple& input) {
    TupleReader<0, Tuple>::read(is, input);
  }
};

template <typename Tuple>
struct Writer<Tuple, typename std::enable_if<(std::tuple_size<Tuple>::value > 0)>::type> {
  static void write(std::ostream& os, Tuple& output) {
    TupleWriter<0, Tuple>::write(os, output);
  }
};

// Specialization for vector types.
template <typename T>
struct Reader<std::vector<T>, void> {
  static void read(std::istream& is, std::vector<T>& input) {
    size_t size;
    Reader<size_t>::read(is, size);
    input.resize(size);
    for (size_t i = 0; i < size; i++) {
      Reader<T>::read(is, input[i]);
    }
  }
};

} // namespace io
