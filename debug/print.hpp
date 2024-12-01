#pragma once
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

namespace debug::detail {
template <typename T> void print(std::optional<T> const& v);
template <typename T> void print(std::vector<T> const& vec);
template <typename T, typename U> void print(std::map<T, U> const& map);
template <std::size_t S, std::size_t I, typename T>
void tuple_print(T const& v);

template <typename T> auto print(T&& val) -> decltype(std::cerr << val) {
  return std::cerr << val;
}
template <typename T>
void print(T const& v,
           std::enable_if_t<std::tuple_size<T>::value != 0>* = nullptr) {
  std::cerr << "(";
  tuple_print<std::tuple_size_v<T>, 0>(v);
  std::cerr << ")";
}
template <typename T> void print(std::vector<T> const& vec) {
  if (vec.size() == 0) {
    std::cerr << "[]";
  } else {
    std::cerr << "[";
    print(vec[0]);
    for (auto i : boost::irange<std::size_t>(1, vec.size())) {
      std::cerr << ", ";
      print(vec[i]);
    }
    std::cerr << "]";
  }
}
template <typename T, typename U> void print(std::map<T, U> const& map) {
  if (map.size() == 0) {
    std::cerr << "{}";
  } else {
    std::cerr << "{";
    auto it = map.begin();
    print(*it);
    while (++it != map.end()) {
      std::cerr << ", ";
      print(*it);
    }
    std::cerr << "}";
  }
}

template <typename T> void print(std::optional<T> const& v) {
  if (v) {
    print(*v);
  } else {
    std::cerr << "<none>";
  }
}
template <std::size_t S, std::size_t I, typename T>
void tuple_print(T const& v) {
  print(std::get<I>(v));
  if constexpr (I + 1 != S) {
    std::cerr << ", ";
    tuple_print<S, I + 1>(v);
  }
}
} // namespace debug::detail

namespace debug {
void println() { std::cerr << std::endl; }
template <typename T> void println(T const& val) {
  detail::print(val);
  std::cerr << std::endl;
}
template <typename T, typename... Ts>
void println(T const& val, Ts const&... vals) {
  detail::print(val);
  std::cerr << " ";
  println(vals...);
}
} // namespace debug
