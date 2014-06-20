#pragma once

#include <cstddef>
#include <functional>
#include <iostream>
#include <tuple>

#include "Common/Io.h"
#include "Common/IntegerSequence.h"

namespace detail {

template <typename Function, typename Tuple, std::size_t... Indices>
auto invoke(Function function, const Tuple& tuple, cxx14::index_sequence<Indices...>)
-> decltype(function(std::get<Indices>(tuple)...)) {
  return function(std::get<Indices>(tuple)...);
}

template <typename Out, typename... In>
std::function<void()> getSolverWrapper(Out (*solver)(In...), std::istream& is, std::ostream& os) {
  return [solver, &is, &os]() {
    is.exceptions(std::istream::failbit);

    std::size_t n;
    io::Reader<std::size_t>::read(is, n);
    for (std::size_t i = 1; i <= n; i++) {
      using Input = std::tuple<typename std::decay<In>::type...>;
      Input input;
      io::Reader<Input>::read(is, input);
      using Output = typename std::decay<Out>::type;
      Output output = detail::invoke(solver, input, cxx14::index_sequence_for<In...>());
      os << "Case #" << i << ": ";
      io::Writer<Output>::write(os, output);
      os << '\n';
    }
  };
}

} // namespace detail

template <typename Out, typename... In>
FunctionalExecutor::FunctionalExecutor(Out (*solver)(In...), void (*initializer)(), std::istream& is, std::ostream& os) {
  solverWrapper_ = detail::getSolverWrapper(solver, is, os);
  initializer_ = initializer;
}
