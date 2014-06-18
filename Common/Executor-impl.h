#pragma once

#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

#include "Common/Io.h"

namespace {

template <typename Function, typename... In>
using FunctionOutput = decltype(std::declval<Function>()(std::declval<In>()...));

template <typename Function, typename... In, typename... Unpacked>
typename std::enable_if<sizeof...(Unpacked) == sizeof...(In), FunctionOutput<Function, In...>>::type
invoke(Function function, const std::tuple<In...>& /*args*/, const Unpacked&... unpacked) {
  return function(unpacked...);
}

template <typename Function, typename... In, typename... Unpacked>
typename std::enable_if<sizeof...(Unpacked) < sizeof...(In), FunctionOutput<Function, In...>>::type
invoke(Function function, const std::tuple<In...>& args, const Unpacked&... unpacked) {
  return invoke(function, args, unpacked..., std::get<sizeof...(Unpacked)>(args));
}

} // namespace

template <typename Out, typename... In>
FunctionalExecutor::FunctionalExecutor(Out (*solver)(In...), std::istream& is, std::ostream& os) {
  setSolverWrapper(solver, is, os);
}

template <typename Out, typename... In>
FunctionalExecutor::FunctionalExecutor(Out (*solver)(In...), void (*initializer)(), std::istream& is, std::ostream& os) {
  setSolverWrapper(solver, is, os);
  initializerWrapper_ = initializer;
}

template <typename Out, typename... In>
void FunctionalExecutor::setSolverWrapper(Out (*solver)(In...), std::istream& is, std::ostream& os) {
  solverWrapper_ = [solver, &is, &os]() {
    is.exceptions(std::istream::failbit);

    size_t n;
    io::Reader<size_t>::read(is, n);
    for (size_t i = 1; i <= n; i++) {
      using Input = std::tuple<typename std::decay<In>::type...>;
      Input input;
      io::Reader<Input>::read(is, input);
      using Output = typename std::decay<Out>::type;
      Output output = invoke(solver, input);
      os << "Case #" << i << ": ";
      io::Writer<Output>::write(os, output);
      os << '\n';
    }
  };
}

