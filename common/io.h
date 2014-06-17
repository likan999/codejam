#pragma once

#include <iostream>
#include <tuple>

namespace io {

template <typename... In>
void read(std::istream& is, std::tuple<In...>& input);

template <typename T>
void write(std::ostream& os, const T& output);

template <typename... Out>
void write(std::ostream& os, const std::tuple<Out...>& out);

} // namespace io

#include "io-impl.h"
