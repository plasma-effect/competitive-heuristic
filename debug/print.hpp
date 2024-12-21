#pragma once
#include <atcoder/all>
#include <bits/stdc++.h>
#include <boost/range/irange.hpp>

namespace local_print {
template <typename T>
concept output_able = requires(T a) { std::declval<std::ostream&>() << a; };

// declare
template <std::ranges::input_range T>
void print(std::ostream& ost, T const& ar);
template <std::integral Int>
void print(std::ostream& ost, boost::integer_range<Int> const& rng);
template <typename T, std::size_t N>
void print(std::ostream& ost, T const (&ar)[N]);
template <typename T>
void print(std::ostream& ost, std::optional<T> const& opt);
template <typename T0, typename T1>
void print(std::ostream& ost, std::pair<T0, T1> const& p);
template <typename... Ts>
void print(std::ostream& ost, std::tuple<Ts...> const& t);
template <std::size_t Size, std::size_t Index, typename Tuple>
void tuple_print(std::ostream& ost, Tuple const& t);
template <int Mod>
void print(std::ostream& ost, atcoder::static_modint<Mod> const& v);

// define
template <output_able T> void print(std::ostream& ost, T const& v) { ost << v; }
template <std::size_t N> void print(std::ostream& ost, const char (&str)[N]) {
  ost << str;
}
void print(std::ostream& ost, std::string const& str) { ost << str; }
template <std::ranges::input_range T>
void print(std::ostream& ost, T const& ar) {
  auto ite = std::ranges::begin(ar);
  auto end = std::ranges::end(ar);
  if (ite == end) {
    ost << "{}";
  } else {
    ost << "{";
    print(ost, *ite);
    for (++ite; ite != end; ++ite) {
      ost << ", ";
      print(ost, *ite);
    }
    ost << "}";
  }
}
template <std::integral Int>
void print(std::ostream& ost, boost::integer_range<Int> const& rng) {
  auto ite = std::ranges::begin(rng);
  auto end = std::ranges::end(rng);
  if (ite == end) {
    ost << "{}";
  } else {
    ost << "{";
    print(ost, *ite);
    for (++ite; ite != end; ++ite) {
      ost << ", ";
      print(ost, *ite);
    }
    ost << "}";
  }
}
template <typename T, std::size_t N>
void print(std::ostream& ost, T const (&ar)[N]) {
  ost << "{";
  print(ost, ar[0]);
  for (auto i : boost::irange<std::size_t>(1, N)) {
    ost << ", ";
    print(ost, ar[i]);
  }
  ost << "}";
}
template <typename T>
void print(std::ostream& ost, std::optional<T> const& opt) {
  if (opt) {
    print(ost, *opt);
  } else {
    ost << "<none>";
  }
}
template <typename T0, typename T1>
void print(std::ostream& ost, std::pair<T0, T1> const& p) {
  ost << "(";
  print(ost, p.first);
  ost << ", ";
  print(ost, p.second);
  ost << ")";
}
template <typename... Ts>
void print(std::ostream& ost, std::tuple<Ts...> const& t) {
  ost << "(";
  print(ost, std::get<0>(t));
  tuple_print<sizeof...(Ts), 1>(ost, t);
  ost << ")";
}
template <std::size_t Size, std::size_t Index, typename Tuple>
void tuple_print(std::ostream& ost, Tuple const& t) {
  ost << ", ";
  print(ost, std::get<Index>(t));
  if constexpr (Index + 1 != Size) {
    tuple_print<Size, Index + 1>(ost, t);
  }
}
template <int Mod>
void print(std::ostream& ost, atcoder::static_modint<Mod> const& v) {
  ost << v.val();
}
} // namespace local_print

namespace debug {
void println() { std::cerr << std::endl; }
template <typename T> void println(T const& val) {
  local_print::print(std::cerr, val);
  std::cerr << std::endl;
}
template <typename T, typename... Ts>
void println(T const& val, Ts const&... vals) {
  local_print::print(std::cerr, val);
  std::cerr << " ";
  println(vals...);
}
} // namespace debug
