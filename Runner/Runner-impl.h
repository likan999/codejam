#pragma once

#include <cstddef>
#include <functional>
#include <iostream>
#include <tuple>

#include "Common/IntegerSequence.h"
#include "Runner/Io.h"

namespace detail {

template <typename Function, typename Tuple, std::size_t... Indices>
auto invoke(Function function, const Tuple& tuple, cxx14::index_sequence<Indices...>)
-> decltype(function(std::get<Indices>(tuple)...)) {
  return function(std::get<Indices>(tuple)...);
}

template <typename Out, typename... In>
std::function<void()> getRunnerWrapper(Out (*runner)(In...), std::istream& is, std::ostream& os) {
  return [runner, &is, &os]() {
    is.exceptions(std::istream::failbit);

    std::size_t n;
    io::Reader<std::size_t>::read(is, n);
    for (std::size_t i = 1; i <= n; i++) {
      using Input = std::tuple<typename std::decay<In>::type...>;
      Input input;
      io::Reader<Input>::read(is, input);
      using Output = typename std::decay<Out>::type;
      Output output = detail::invoke(runner, input, cxx14::index_sequence_for<In...>());
      os << "Case #" << i << ": ";
      io::Writer<Output>::write(os, output);
      os << '\n';
    }
  };
}

} // namespace detail

template <typename Out, typename... In>
FunctionRunner::FunctionRunner(Out (*runner)(In...), void (*initializer)(), std::istream& is, std::ostream& os) {
  runnerWrapper_ = detail::getRunnerWrapper(runner, is, os);
  initializer_ = initializer;
}
