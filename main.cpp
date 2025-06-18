#include "atcoder/all"
#include "bits/stdc++.h"
#include "boost/container/static_vector.hpp"
#include "boost/range/irange.hpp"
#ifdef LOCAL_DEBUG
namespace debug::internal {
[[noreturn]] inline void assertion_fail(const char* expr) {
  auto st = std::stacktrace::current(1);
  std::cerr << "assertion failed: \"" << expr << "\"\nwhere: " << st[0] << "\n";
  for (auto it = std::next(st.begin()); it != st.end(); ++it) {
    if (it->source_line() != 0) {
      std::cerr << " from: " << *it << "\n";
    }
  }
  std::cerr << std::flush;
  throw std::logic_error("assertion failed");
}
} // namespace debug::internal
#define CL_ASSERT(expr)                                                        \
  (static_cast<bool>(expr) ? void(0) : debug::internal::assertion_fail(#expr))
#else
#define CL_ASSERT(expr) assert(expr)
#endif
namespace common {
template <typename T> constexpr auto max_v = std::numeric_limits<T>::max();
template <typename T> constexpr auto min_v = std::numeric_limits<T>::min();
template <std::integral Int> auto irange(Int first, Int last) {
  CL_ASSERT(std::cmp_less_equal(first, last));
  return std::views::iota(first, last);
}
template <std::integral Int> auto irange(Int last) {
  return irange(Int(0), last);
}
template <typename T> using pair = std::pair<T, T>;
template <typename T> class dual_array {
  std::vector<T> inside_;
  std::size_t dim0, dim1;
public:
  dual_array(std::size_t d0, std::size_t d1)
      : inside_(d0 * d1), dim0(d0), dim1(d1) {}
  template <std::integral Int0, std::integral Int1>
  T& operator()(Int0 i0, Int1 i1) {
    CL_ASSERT(std::cmp_greater_equal(i0, 0) && std::cmp_less(i0, dim0));
    CL_ASSERT(std::cmp_greater_equal(i1, 0) && std::cmp_less(i1, dim1));
    return inside_[i0 * dim1 + i1];
  }
  template <std::integral Int0, std::integral Int1>
  T const& operator()(Int0 i0, Int1 i1) const {
    CL_ASSERT(std::cmp_greater_equal(i0, 0) && std::cmp_less(i0, dim0));
    CL_ASSERT(std::cmp_greater_equal(i1, 0) && std::cmp_less(i1, dim1));
    return inside_[i0 * dim1 + i1];
  }
  common::pair<std::size_t> dimensions() const {
    return {dim0, dim1};
  }
  std::size_t size() const {
    return dim0 * dim1;
  }
};
template <> class dual_array<bool> : public dual_array<std::uint8_t> {
public:
  using dual_array<std::uint8_t>::dual_array;
};
template <typename T, typename F = std::greater<>>
using priority_queue = std::priority_queue<T, std::vector<T>, F>;
constexpr bool is_debug() {
#ifdef LOCAL_DEBUG
  return true;
#else
  return false;
#endif
}
} // namespace common
namespace common::internal {
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
  ~print_base_t() {
    ost.flags(base_flags);
  }
  print_base_t& operator<<(std::string const& str) {
    ost << str;
    return *this;
  }
  print_base_t& operator<<(std::string_view const& view) {
    ost << view;
    return *this;
  }
  print_base_t& operator<<(const char* str) {
    ost << str;
    return *this;
  }
  template <int mod>
  print_base_t& operator<<(atcoder::static_modint<mod> const& val) {
    ost << val.val();
    return *this;
  }
  template <stdstream_able T> print_base_t& operator<<(T const& v) {
    ost << v;
    return *this;
  }
  template <std::input_iterator It> void print_ite(It first, It last) {
    ost << rng_dec.prefix;
    if (first != last) {
      *this << *first;
      for (++first; first != last; ++first) {
        ost << rng_dec.delim;
        *this << *first;
      }
    }
    ost << rng_dec.suffix;
  }
  template <std::ranges::input_range T> print_base_t& operator<<(T const& rng) {
    print_ite(rng.begin(), rng.end());
    return *this;
  }
  template <typename T, std::size_t N>
  print_base_t& operator<<(T const (&ar)[N]) {
    print_ite(std::ranges::begin(ar), std::ranges::end(ar));
    return *this;
  }
  template <std::size_t S, std::size_t I, typename T>
  void tuple_print(T const& t) {
    if constexpr (I == 0) {
      ost << tpl_dec.prefix;
    } else {
      ost << tpl_dec.delim;
    }
    *this << std::get<I>(t);
    if constexpr (I + 1 != S) {
      tuple_print<S, I + 1>(t);
    } else {
      ost << tpl_dec.suffix;
    }
  }
  template <typename T0, typename T1>
  print_base_t& operator<<(std::pair<T0, T1> const& p) {
    tuple_print<2, 0>(p);
    return *this;
  }
  template <typename... Ts>
  print_base_t& operator<<(std::tuple<Ts...> const& t) {
    tuple_print<sizeof...(Ts), 0>(t);
    return *this;
  }
  template <typename T> print_base_t& operator<<(std::optional<T> const& opt) {
    if (opt) {
      *this << *opt;
    } else {
      ost << "<nullopt>";
    }
    return *this;
  }
  template <typename T>
  print_base_t& operator<<(common::dual_array<T> const& ar) {
    auto [H, W] = ar.dimensions();
    const char* outer_delim = "";
    ost << rng_dec.prefix;
    for (auto i : common::irange(H)) {
      ost << std::exchange(outer_delim, rng_dec.delim) << rng_dec.prefix;
      const char* inner_delim = "";
      for (auto j : common::irange(W)) {
        ost << std::exchange(inner_delim, rng_dec.delim);
        *this << ar(i, j);
      }
      ost << rng_dec.suffix;
    }
    ost << rng_dec.suffix;
    return *this;
  }
  print_base_t& operator<<(common::dual_array<bool> const& ar) {
    auto [H, W] = ar.dimensions();
    const char* outer_delim = "";
    ost << rng_dec.prefix;
    for (auto i : common::irange(H)) {
      ost << std::exchange(outer_delim, rng_dec.delim) << rng_dec.prefix;
      const char* inner_delim = "";
      for (auto j : common::irange(W)) {
        ost << std::exchange(inner_delim, rng_dec.delim);
        ost << bool(ar(i, j));
      }
      ost << rng_dec.suffix;
    }
    ost << rng_dec.suffix;
    return *this;
  }
  void set_range_prefix(const char* new_prefix) {
    rng_dec.prefix = new_prefix;
  }
  void set_range_suffix(const char* new_suffix) {
    rng_dec.suffix = new_suffix;
  }
  void set_range_delim(const char* new_delim) {
    rng_dec.delim = new_delim;
  }
  void set_tuple_prefix(const char* new_prefix) {
    tpl_dec.prefix = new_prefix;
  }
  void set_tuple_suffix(const char* new_suffix) {
    tpl_dec.suffix = new_suffix;
  }
  void set_tuple_delim(const char* new_delim) {
    tpl_dec.delim = new_delim;
  }
};
template <typename T>
constexpr bool is_std_manip_v =
    std::is_same_v<T, decltype(std::setbase(std::declval<int>()))> ||
    std::is_same_v<T, decltype(std::setfill(std::declval<char>()))> ||
    std::is_same_v<T, decltype(std::setprecision(std::declval<int>()))> ||
    std::is_same_v<T, decltype(std::setw(std::declval<int>()))> ||
    std::is_convertible_v<T, std::ios_base& (*)(std::ios_base&)>;
template <bool> void print(print_base_t&) {}
template <bool put_blank, typename T, typename... Ts>
void print(print_base_t& pb, T const& arg, Ts const&... args) {
  if constexpr (put_blank) {
    pb << " ";
  }
  pb << arg;
  print<!is_std_manip_v<std::remove_cv_t<T>>>(pb, args...);
}
} // namespace common::internal
namespace debug {
inline void println() {
  std::cerr << std::endl;
}
template <typename... Ts> void println(Ts const&... args) {
  common::internal::print_base_t pb(std::cerr);
  pb.set_range_prefix("{");
  pb.set_range_suffix("}");
  pb.set_range_delim(", ");
  pb.set_tuple_prefix("(");
  pb.set_tuple_suffix(")");
  pb.set_tuple_delim(", ");
  common::internal::print<false>(pb, args...);
  std::cerr << std::endl;
}
} // namespace debug
#ifdef LOCAL_DEBUG
#define DEBUG_PRINT(...) debug::println(__LINE__, ":", __VA_ARGS__)
#else
#define DEBUG_PRINT(...) (void)(0)
#endif
namespace heuristic {
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
  auto begin() const {
    return cont.begin();
  }
  auto end() const {
    return cont.end();
  }
};
template <typename T, std::size_t H, std::size_t W> class static_dual_array {
  std::array<std::array<T, W>, H> inside_;
public:
  static_dual_array() : inside_{} {};
  template <std::integral Int0, std::integral Int1>
  T& operator()(Int0 i0, Int1 i1) {
    CL_ASSERT(std::cmp_greater_equal(i0, 0) && std::cmp_less(i0, H));
    CL_ASSERT(std::cmp_greater_equal(i1, 0) && std::cmp_less(i1, W));
    return inside_[i0][i1];
  }
  template <std::integral Int0, std::integral Int1>
  T const& operator()(Int0 i0, Int1 i1) const {
    CL_ASSERT(std::cmp_greater_equal(i0, 0) && std::cmp_less(i0, H));
    CL_ASSERT(std::cmp_greater_equal(i1, 0) && std::cmp_less(i1, W));
    return inside_[i0][i1];
  }
  std::pair<std::size_t, std::size_t> dimensions() const {
    return {H, W};
  }
  std::size_t size() const {
    return H * W;
  }
  friend common::internal::print_base_t&
  operator<<(common::internal::print_base_t& pb, static_dual_array const& ar) {
    return pb << ar.inside_;
  }
};
} // namespace heuristic
namespace heuristic {
template <typename T, std::size_t H, std::size_t W> class grid_bfs_queue {
  using internal_t = std::pair<int, heuristic::static_dual_array<int, H, W>>;
  using internal_ptr = std::unique_ptr<internal_t>;
  static inline std::vector<internal_ptr> grids;
  internal_ptr grid_ptr;
  std::queue<std::tuple<int, int, T>> queue;
  bool check_front(bool pop_flag) {
    auto& [count, flags] = *grid_ptr;
    while (queue.size()) {
      auto& [i, j, v] = queue.front();
      if (std::cmp_less(i, 0) || std::cmp_greater_equal(i, H) ||
          std::cmp_less(j, 0) || std::cmp_greater_equal(j, W)) {
        queue.pop();
      } else if (flags(i, j) == count) {
        queue.pop();
      } else {
        if (pop_flag) {
          flags(i, j) = count;
        }
        return true;
      }
    }
    return false;
  }
public:
  grid_bfs_queue() : grid_ptr(), queue() {
    if (grids.size()) {
      grid_ptr = std::move(grids.back());
      grids.pop_back();
    } else {
      grid_ptr = std::make_unique<internal_t>(
          std::pair(0, heuristic::static_dual_array<int, H, W>()));
    }
    ++grid_ptr->first;
  }
  ~grid_bfs_queue() {
    grids.push_back(std::move(grid_ptr));
  }
  void push(int i, int j, T&& arg) {
    queue.emplace(i, j, std::move(arg));
  }
  void push(int i, int j, const T& arg) {
    queue.emplace(i, j, arg);
  }
  template <typename... Args> void emplace(int i, int j, Args&&... args) {
    push(i, j, T(std::forward<Args>(args)...));
  }
  std::optional<std::tuple<int, int, T>> pop() {
    if (check_front(true)) {
      auto&& ret = std::move(queue.front());
      queue.pop();
      return std::make_optional(std::move(ret));
    } else {
      return std::nullopt;
    }
  }
};
} // namespace heuristic
namespace heuristic {
namespace internal {
inline std::mt19937& get_common_engine() {
  thread_local std::mt19937 engine{};
  return engine;
}
} // namespace internal
template <typename T> auto make_uniform_int_distribution(T min, T max) {
  auto& engine = internal::get_common_engine();
  std::uniform_int_distribution<T> dist(min, max);
  return [&engine, dist]() mutable {
    return dist(engine);
  };
}
template <typename T> auto make_uniform_int_distribution(T max) {
  return make_uniform_int_distribution<T>(0, max);
}
inline double generate_canonical() {
  auto& engine = internal::get_common_engine();
  constexpr auto digits = std::numeric_limits<double>::digits;
  return std::generate_canonical<double, digits>(engine);
}
template <typename Rng> void shuffle(Rng& rng) {
  std::ranges::shuffle(rng, internal::get_common_engine());
}
template <std::random_access_iterator It> void shuffle(It first, It last) {
  std::shuffle(first, last, internal::get_common_engine());
}
} // namespace heuristic
namespace heuristic {
using time_t = std::chrono::milliseconds;
auto get_time() {
  using namespace std::chrono;
  thread_local const auto start = system_clock::now();
  return duration_cast<milliseconds>(system_clock::now() - start);
}
namespace internal {
template <typename Derived> class time_control_base {
  time_t time_limit_, current_;
  std::size_t update_frequency_, update_count_;
public:
  time_control_base(time_t time_limit, std::size_t ufreq = 1)
      : time_limit_(time_limit), current_(get_time()), update_frequency_(ufreq),
        update_count_() {}
  operator bool() {
    if (++update_count_ == update_frequency_) {
      update_count_ = 0;
      current_ = get_time();
      static_cast<Derived&>(*this).update(time_limit_, current_);
    }
    return current_ < time_limit_;
  }
};
} // namespace internal
struct time_control_t : internal::time_control_base<time_control_t> {
  void update(time_t const&, time_t const&) {}
  using time_control_base::time_control_base;
};
class time_control_with_annealing
    : public internal::time_control_base<time_control_with_annealing> {
  using time_control_base::time_control_base;
public:
  double T1, dT, T;
  time_control_with_annealing(time_t time_limit, std::size_t ufreq, double t0,
                              double t1)
      : time_control_base(time_limit, ufreq), T1(t1), dT(t0 / t1), T(t0) {}
  time_control_with_annealing(time_t time_limit, double t0, double t1)
      : time_control_with_annealing(time_limit, 1, t0, t1) {}
  void update(time_t const& time_limit, time_t const& current) {
    auto nt = double(current.count()) / double(time_limit.count());
    T = T1 * std::pow(dT, 1 - nt);
  }
  double annealing_threshold(double diff) {
    return std::exp(diff / T);
  }
  bool transition_check(double diff) {
    if (diff > 0) {
      return true;
    } else {
      return generate_canonical() < annealing_threshold(diff);
    }
  }
};
} // namespace heuristic
namespace common {
template <std::integral T> constexpr T abs(T const& v) {
  if constexpr (std::is_signed_v<T>) {
    return v < 0 ? -v : v;
  } else {
    return v;
  }
}
template <typename T> std::vector<T> make_factorial_array(std::size_t max) {
  std::vector<T> ret(max + 1);
  ret[0] = T(1);
  for (auto i : common::irange(max)) {
    ret[i + 1] = ret[i] * T(i + 1);
  }
  return ret;
}
} // namespace common
namespace common {
inline void println() {
  std::cout << "\n";
}
template <typename... Ts> void println(Ts const&... args) {
  common::internal::print_base_t pb(std::cout);
  common::internal::print<false>(pb, args...);
  std::cout << "\n";
}
} // namespace common
#ifdef LOCAL_DEBUG
#include "grid_slider/grid.hpp"
#else
namespace debug {
template <std::size_t, std::size_t> class grid {
public:
  template <std::integral Int0, std::integral Int1>
  void set_both(Int0 const&, Int1 const&, std::optional<int> const&,
                const char*) {}
  template <std::integral Int0, std::integral Int1>
  void set_value(Int0 const&, Int1 const&, std::optional<int> const&) {}
  template <std::integral Int0, std::integral Int1>
  void set_color(Int0 const&, Int1 const&, const char*) {}
};
} // namespace debug
#endif
#define IGNORE [[maybe_unused]] auto _

void Main() {}

int main() {
  std::cin.tie(nullptr);
  std::cin.sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(15);
  Main();
  std::cout << std::flush;
}
