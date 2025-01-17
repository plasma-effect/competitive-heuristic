#include "debug/print.hpp"
#include <atcoder/all>
#include <bits/stdc++.h>
#include <boost/format.hpp>
#include <boost/range/irange.hpp>

namespace common {
namespace detail {
template <local_print::output_able T> void print(T a) { std::cout << a; }
void print(std::string const& str) { std::cout << str; }
void print(const char* str) { std::cout << str; }
template <std::size_t N> void print(const char (&str)[N]) { std::cout << str; }
template <std::input_iterator T> void print(T ite, T last) {
  if (ite != last) {
    print(*ite);
    for (++ite; ite != last; ++ite) {
      std::cout << " ";
      print(*ite);
    }
  }
}
template <typename T> void print(std::vector<T> const& vec) {
  print(std::ranges::begin(vec), std::ranges::end(vec));
}
template <std::integral Int> void print(boost::integer_range<Int> const& rng) {
  print(std::ranges::begin(rng), std::ranges::end(rng));
}
template <typename T, std::size_t N> void print(T const (&ar)[N]) {
  print(std::ranges::begin(ar), std::ranges::end(ar));
}
template <typename T, std::size_t N> void print(std::array<T, N> const& ar) {
  print(std::ranges::begin(ar), std::ranges::end(ar));
}
template <typename T0, typename T1> void print(std::pair<T0, T1> const& p) {
  print(p.first);
  std::cout << " ";
  print(p.second);
}
} // namespace detail
void println() { std::cout << std::endl; }
template <typename T> void println(T const& val) {
  detail::print(val);
  std::cout << std::endl;
}
template <typename T, typename... Ts>
void println(T const& val, Ts const&... vals) {
  detail::print(val);
  std::cout << " ";
  println(vals...);
}
} // namespace common

void Main(int argc, const char* argv[]) {
  int C = argc == 1 ? 1 : std::stoi(argv[1]);
  std::mt19937 engine;
  const int N = 26, D = 365;
  std::uniform_int_distribution<> dec{0, 100}, score{0, 20000};
  for (auto i : boost::irange(C)) {
    char filename[100];
    std::sprintf(filename, "samples/%04d.txt", i);
    std::fstream fst(filename, std::ios::out);
    fst << D << "\n";
    for (auto j : boost::irange(N)) {
      if (j != 0) {
        fst << " ";
      }
      fst << dec(engine);
    }
    fst << "\n";
    for ([[maybe_unused]] auto j : boost::irange(D)) {
      for ([[maybe_unused]] auto k : boost::irange(N)) {
        if (k != 0) {
          fst << " ";
        }
        fst << score(engine);
      }
      fst << "\n";
    }
    fst << std::flush;
  }
}

int main(int argc, const char* argv[]) {
  std::cin.tie(nullptr);
  std::cin.sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(15);
  Main(argc, argv);
}
