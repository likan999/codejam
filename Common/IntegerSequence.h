#pragma once

#include <cstddef>

/**
 * Implementation of std::integer_sequence before gcc 4.9 is available.
 */

namespace cxx14 {

template <typename T, T... Ints>
struct integer_sequence {
  using value_type = T;
  static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
};

template <std::size_t... Ints>
using index_sequence = integer_sequence<std::size_t, Ints...>;

namespace detail {

template <typename T, std::size_t N, T... Ints>
struct make_integer_sequence_helper_ {
  using type = typename make_integer_sequence_helper_<T, N - 1, Ints..., sizeof...(Ints)>::type;
};

template <typename T, T... Ints>
struct make_integer_sequence_helper_<T, 0, Ints...> {
  using type = integer_sequence<T, Ints...>;
};

} // namespace detail

template<class T, T N>
using make_integer_sequence = typename detail::make_integer_sequence_helper_<T, N>::type;

template<std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

template<class... T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;

} // namespace cxx14

namespace ktl {

template <std::size_t N, std::size_t... I> struct NthIndex;

template <std::size_t N, std::size_t First, std::size_t... I>
struct NthIndex<N, First, I...> {
  static_assert(N <= sizeof...(I), "Index out of bound");
  static constexpr std::size_t value = NthIndex<N - 1, I...>::value;
};

template <std::size_t First, std::size_t... I>
struct NthIndex<0, First, I...> {
  static constexpr std::size_t value = First;
};

} // namespace ktl
