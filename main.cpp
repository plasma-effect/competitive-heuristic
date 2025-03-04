#include "bits/stdc++.h"
#include <atcoder/all>
#include <boost/container/static_vector.hpp>
#include <boost/range/irange.hpp>
#ifdef LOCAL_DEBUG
#include "debug/print.hpp"
#include "grid_slider/grid.hpp"
#else
namespace debug {
template <typename... Ts> void println(Ts const&...) {}
} // namespace debug
#endif
namespace sch = std::chrono;

namespace common {
template <typename T> constexpr auto max_v = std::numeric_limits<T>::max();
template <typename T> constexpr auto min_v = std::numeric_limits<T>::min();

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

template <typename T> using pair = std::pair<T, T>;

template <typename T> class dual_array {
  std::vector<T> inside_;
  std::size_t dim0, dim1;

public:
  dual_array(std::size_t d0, std::size_t d1)
      : inside_(d0 * d1), dim0(d0), dim1(d1) {}
  T& operator()(int i0, int i1) {
    assert(0 <= i0 && std::cmp_less(i0, dim0));
    assert(0 <= i1 && std::cmp_less(i1, dim1));
    return inside_[i0 * dim1 + i1];
  }
  T const& operator()(int i0, int i1) const {
    assert(0 <= i0 && std::cmp_less(i0, dim0));
    assert(0 <= i1 && std::cmp_less(i1, dim1));
    return inside_[i0 * dim1 + i1];
  }
  T& operator()(std::size_t i0, std::size_t i1) {
    assert(std::cmp_less(i0, dim0));
    assert(std::cmp_less(i1, dim1));
    return inside_[i0 * dim1 + i1];
  }
  T const& operator()(std::size_t i0, std::size_t i1) const {
    assert(std::cmp_less(i0, dim0));
    assert(std::cmp_less(i1, dim1));
    return inside_[i0 * dim1 + i1];
  }
  common::pair<std::size_t> dimensions() const { return {dim0, dim1}; }
  std::size_t size() const { return dim0 * dim1; }
};

template <typename T, typename F = std::greater<>>
using priority_queue = std::priority_queue<T, std::vector<T>, F>;
} // namespace common

namespace print_detail {
template <typename T>
concept stdstream_able = requires(T a) { std::declval<std::ostream&>() << a; };

class print_base_t {
  std::ios_base::fmtflags base_flags;
  std::ostream& ost;
  struct dec_t {
    const char* prefix = "";
    const char* suffix = "";
    const char* delim = " ";
  };
  dec_t rng_dec, tpl_dec;

public:
  print_base_t(std::ostream& os)
      : base_flags(os.flags()), ost(os), rng_dec{}, tpl_dec{} {}
  ~print_base_t() { ost.setf(base_flags); }

  void print(std::string const& str) { ost << str; }
  void print(std::string_view const& view) { ost << view; }
  void print(const char* str) { ost << str; }
  template <stdstream_able T> void print(T const& v) { ost << v; }
  template <std::input_iterator It> void print(It first, It last) {
    ost << rng_dec.prefix;
    if (first != last) {
      print(*first);
      for (++first; first != last; ++first) {
        ost << rng_dec.delim;
        print(*first);
      }
    }
    ost << rng_dec.suffix;
  }
  template <std::integral Int>
  void print(common::integer_range<Int> const& rng) {
    print(rng.begin(), rng.end());
  }
  template <std::ranges::input_range T> void print(T const& rng) {
    print(rng.begin(), rng.end());
  }
  template <typename T, std::size_t N> void print(T const (&ar)[N]) {
    print(std::ranges::begin(ar), std::ranges::end(ar));
  }
  template <std::size_t S, std::size_t I, typename T>
  void tuple_print(T const& t) {
    if constexpr (I == 0) {
      ost << tpl_dec.prefix;
    } else {
      ost << tpl_dec.delim;
    }
    print(std::get<I>(t));
    if constexpr (I + 1 != S) {
      tuple_print<S, I + 1>(t);
    } else {
      ost << tpl_dec.suffix;
    }
  }
  template <typename T0, typename T1> void print(std::pair<T0, T1> const& p) {
    tuple_print<2, 0>(p);
  }
  template <typename... Ts> void print(std::tuple<Ts...> const& t) {
    tuple_print<sizeof...(Ts), 0>(t);
  }
  void set_range_prefix(const char* new_prefix) { rng_dec.prefix = new_prefix; }
  void set_range_suffix(const char* new_suffix) { rng_dec.suffix = new_suffix; }
  void set_range_delim(const char* new_delim) { rng_dec.delim = new_delim; }
  void set_tuple_prefix(const char* new_prefix) { tpl_dec.prefix = new_prefix; }
  void set_tuple_suffix(const char* new_suffix) { tpl_dec.suffix = new_suffix; }
  void set_tuple_delim(const char* new_delim) { tpl_dec.delim = new_delim; }
};
} // namespace print_detail

namespace common {
namespace detail {
template <typename T>
constexpr bool is_std_manip_v =
    std::is_same_v<T, decltype(std::setbase(std::declval<int>()))> ||
    std::is_same_v<T, decltype(std::setfill(std::declval<char>()))> ||
    std::is_same_v<T, decltype(std::setprecision(std::declval<int>()))> ||
    std::is_same_v<T, decltype(std::setw(std::declval<int>()))> ||
    std::is_convertible_v<T, std::ios_base& (*)(std::ios_base&)>;
template <bool> void print(print_detail::print_base_t&) {}
template <bool put_blank, typename T, typename... Ts>
void print(print_detail::print_base_t& pb, T const& arg, Ts const&... args) {
  if constexpr (put_blank) {
    pb.print(" ");
  }
  pb.print(arg);
  print<!is_std_manip_v<std::remove_cv_t<T>>>(pb, args...);
}
} // namespace detail
inline void println() { std::cout << "\n"; }
template <typename... Ts> void println(Ts const&... args) {
  print_detail::print_base_t pb(std::cout);
  detail::print<false>(pb, args...);
  std::cout << "\n";
}
} // namespace common

// For Heuristic
namespace heuristic {
auto get_time() {
  static const auto start = sch::system_clock::now();
  return sch::duration_cast<sch::milliseconds>(sch::system_clock::now() -
                                               start);
}

namespace detail {
std::mt19937& get_common_engine() {
  static std::mt19937 engine{};
  return engine;
}
} // namespace detail
template <typename T> auto make_uniform_int_distribution(T min, T max) {
  auto& engine = detail::get_common_engine();
  std::uniform_int_distribution<T> dist(min, max);
  return [&engine, dist]() mutable { return dist(engine); };
}
double generate_canonical() {
  auto& engine = detail::get_common_engine();
  constexpr auto digits = std::numeric_limits<double>::digits;
  return std::generate_canonical<double, digits>(engine);
}
template <typename Rng> void shuffle(Rng& rng) {
  std::ranges::shuffle(rng, detail::get_common_engine());
}
template <std::random_access_iterator It> void shuffle(It first, It last) {
  std::shuffle(first, last, detail::get_common_engine());
}

class time_control_t {
  sch::milliseconds time_limit_;
  std::size_t update_frequency_;
  double T1, dT, T;
  std::size_t update_count;
  sch::milliseconds current;

public:
  time_control_t(sch::milliseconds time_limit, std::size_t ufreq = 1,
                 double t0 = 2000, double t1 = 600)
      : time_limit_(time_limit), update_frequency_(ufreq), T1(t1), dT(t0 / t1),
        T{}, update_count(), current(get_time()) {
    auto nt = current.count() / double(time_limit_.count());
    T = T1 * std::pow(dT, 1 - nt);
  }
  operator bool() {
    if (++update_count == update_frequency_) {
      update_count = 0;
      current = get_time();
      auto nt = current.count() / double(time_limit_.count());
      T = T1 * std::pow(dT, 1 - nt);
    }
    return current < time_limit_;
  }

  double annealing_threshold(double diff) { return std::exp(diff / T); }
  bool transition_check(double diff) {
    if (diff > 0) {
      return true;
    } else {
      return generate_canonical() < annealing_threshold(diff);
    }
  }
};

// For Beam Search
template <typename T, std::size_t Capacity, typename Compare = std::greater<>>
class static_priority_container {
  boost::container::static_vector<T, Capacity> cont;
  Compare comp;

public:
  static_priority_container(Compare = {}) : cont{}, comp{} {}
  void push(T value) {
    if (cont.size() < Capacity) {
      cont.push_back(value);
      std::ranges::push_heap(cont, comp);
    } else if (comp(value, cont.front())) {
      std::ranges::pop_heap(cont, comp);
      std::swap(cont.back(), value);
      std::ranges::push_heap(cont, comp);
    }
  }
  auto begin() const { return cont.begin(); }
  auto end() const { return cont.end(); }
};

} // namespace heuristic

void Main() {
  for (int x : common::irange(3)) {
    debug::grid<20, 15> test;
    for (int i : common::irange(x + 2, x + 4)) {
      for (int j : common::irange(x + 2, x + 4)) {
        test.set_color(i, j, "#000000");
      }
    }
  }
}

int main() {
  std::cin.tie(nullptr);
  std::cin.sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(15);
  Main();
  std::cout << std::flush;
}
