#pragma once

#include <iostream>
#include <tuple>
#include <type_traits>
#include <vector>

namespace io {

namespace {

template <size_t Next, typename... In>
typename std::enable_if<Next == sizeof...(In)>::type
readTuple(std::istream& /*is*/, std::tuple<In...>& /*input*/) {
}

template <size_t Next, typename... In>
typename std::enable_if<Next < sizeof...(In)>::type
readTuple(std::istream& is, std::tuple<In...>& input) {
  is >> std::get<Next>(input);
  readTuple<Next + 1>(is, input);
}

template <size_t Next, typename... Out>
typename std::enable_if<Next == sizeof...(Out)>::type
writeTuple(std::ostream& /*os*/, const std::tuple<Out...>& /*output*/) {
}

template <size_t Next, typename... Out>
typename std::enable_if<Next < sizeof...(Out)>::type
writeTuple(std::ostream& os, const std::tuple<Out...>& output) {
  if (Next > 0) {
    os << ' ';
  }
  os << std::get<Next>(output);
  writeTuple<Next + 1>(os, output);
}

template <typename T>
std::istream& operator>> (std::istream& is, std::vector<T>& v) {
  size_t size;
  is >> size;
  v.resize(size);
  for (size_t i = 0; i < size; i++) {
    is >> v[i];
  }
  return is;
}

} // namespace

template <typename... In>
void read(std::istream& is, std::tuple<In...>& input) {
  readTuple<0>(is, input);
}

template <typename T>
void write(std::ostream& os, const T& output) {
  os << output;
}

template <typename... Out>
void write(std::ostream& os, const std::tuple<Out...>& output) {
  writeTuple<0>(os, output);
}

} // namespace io
