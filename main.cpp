#include "bits/stdc++.h"
#include <atcoder/all>
#include <boost/multi_array.hpp>
#include <boost/range/irange.hpp>
#ifdef LOCAL_DEBUG
#include "debug/print.hpp"
#else
namespace debug {
template <typename... Ts> void println(Ts const&...) {}
} // namespace debug
#endif

namespace common {
// common
template <typename T> auto max_v = std::numeric_limits<T>::max();
template <typename T> auto min_v = std::numeric_limits<T>::min();
template <typename Integer>
struct integer_range : boost::integer_range<Integer>, std::ranges::view_base {
  using boost::integer_range<Integer>::integer_range;
};
template <typename Integer> integer_range<Integer> irange(Integer last) {
  return integer_range<Integer>(Integer(0), last);
}
template <typename Integer>
integer_range<Integer> irange(Integer first, Integer last) {
  return integer_range<Integer>(first, last);
}
template <typename T>
std::size_t upper_bound_index(std::vector<T> const& vec, T const& v) {
  return std::ranges::upper_bound(vec, v) - vec.begin();
}
template <typename T>
std::size_t lower_bound_index(std::vector<T> const& vec, T const& v) {
  return std::ranges::lower_bound(vec, v) - vec.begin();
}
template <typename T> using pair = std::pair<T, T>;
template <typename T> T const& chmax(T& lhs, T const& v) {
  return lhs = std::max(lhs, v);
}
template <typename T> T const& chmin(T& lhs, T const& v) {
  return lhs = std::min(lhs, v);
}

// segtree
namespace detail {
constexpr std::plus<> plus;
constexpr std::multiplies<> multiplies;
template <typename T> struct get_val_t {
  T val;
  constexpr get_val_t(T v) : val(v) {}
  constexpr auto operator()() const { return val; }
};
struct max_f_t {
  template <typename T>
  constexpr auto operator()(T const& lhs, T const& rhs) const {
    return std::max(lhs, rhs);
  }
};
constexpr max_f_t max;
template <typename T> constexpr get_val_t<T> get_zero{0};
template <typename T> constexpr get_val_t<T> get_one{1};
template <typename T> constexpr get_val_t<T> get_max{max_v<T>};
} // namespace detail
template <typename T>
using add_segtree = atcoder::segtree<T, detail::plus, detail::get_zero<T>>;
template <typename T>
using mul_segtree = atcoder::segtree<T, detail::multiplies, detail::get_one<T>>;
template <typename T>
using max_segtree = atcoder::segtree<T, detail::max, detail::get_max<T>>;

// Floyd-Warshall Algorithm
namespace detail {
template <typename T>
void local_update(std::optional<T>& base, std::optional<T> a,
                  std::optional<T> b) {
  if (a && b) {
    if (!base) {
      base = *a + *b;
    } else {
      base = std::min(*base, *a + *b);
    }
  }
}
template <typename T> void local_update(T& base, T a, T b) {
  base = std::min(base, a + b);
}
} // namespace detail
template <typename T>
void warshall_floyd(boost::multi_array<T, 2>& data, std::size_t N) {
  for (auto k : boost::irange(N)) {
    for (auto i : boost::irange(N)) {
      for (auto j : boost::irange(N)) {
        detail::local_update(data[i][j], data[i][k], data[k][j]);
      }
    }
  }
}

// for dijkstra
template <typename T>
using p_queue = std::priority_queue<T, std::vector<T>, std::greater<>>;

} // namespace common

namespace sch = std::chrono;

void Main() {}

int main() {
  std::cin.tie(nullptr);
  std::cin.sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(15);
  Main();
}
