// plasma-effect/competitive-library Copyright (c) plasma-effect
// https://github.com/plasma-effect/competitive-library
// plasma-effect/competitive-library released under the MIT license
// see https://github.com/plasma-effect/competitive-library/blob/master/LICENSE
#if !defined(LOCAL_DEBUG) && !defined(LOCAL_MEASURE)
#define NDEBUG
#pragma GCC optimize("O3")
#endif
#include "atcoder/all"
#include "bits/stdc++.h"
#include "boost/container/static_vector.hpp"
#include "boost/range/irange.hpp"
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
  std::array<const char*, 3> get_range_decolater() {
    return {
        rng_dec.prefix,
        rng_dec.suffix,
        rng_dec.delim,
    };
  }
  void set_range_decolater(const char* prefix, const char* suffix,
                           const char* delim) {
    rng_dec.prefix = prefix;
    rng_dec.suffix = suffix;
    rng_dec.delim = delim;
  }
  std::array<const char*, 3> get_tuple_decolater() {
    return {
        tpl_dec.prefix,
        tpl_dec.suffix,
        tpl_dec.delim,
    };
  }
  void set_tuple_decolater(const char* prefix, const char* suffix,
                           const char* delim) {
    tpl_dec.prefix = prefix;
    tpl_dec.suffix = suffix;
    tpl_dec.delim = delim;
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
#ifdef LOCAL_DEBUG
namespace common::debug::internal {
[[noreturn]] inline void assertion_fail(const char* expr) {
  auto st = std::stacktrace::current(1);
  std::cerr << "assertion failed: \"" << expr << "\"\n";
  std::cerr << "where: " << st[0].description();
  std::cerr << ": " << st[0].source_line() << "\n";
  for (auto it = std::next(st.begin()); it != st.end(); ++it) {
    if (it->source_line() != 0) {
      std::cerr << " from: " << it->description();
      std::cerr << ": " << it->source_line() << "\n";
    }
  }
  std::cerr << std::flush;
  throw std::logic_error("assertion failed");
}
} // namespace common::debug::internal
#define CL_ASSERT(expr)                                                        \
  (static_cast<bool>(expr) ? void(0)                                           \
                           : common::debug::internal::assertion_fail(#expr))
#else
#define CL_ASSERT(expr) assert(expr)
#endif
namespace common {
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
  template <std::integral Int0, std::integral Int1> T& at(Int0 i0, Int1 i1) {
    if (std::cmp_less(i0, 0) || std::cmp_greater_equal(i0, dim0)) [[unlikely]] {
      throw std::out_of_range("argument 1 of dual_array::at is out of range");
    } else if (std::cmp_less(i1, 0) || std::cmp_greater_equal(i1, dim1))
        [[unlikely]] {
      throw std::out_of_range("argument 2 of dual_array::at is out of range");
    }
    return inside_[i0 * dim1 + i1];
  }
  template <std::integral Int0, std::integral Int1>
  T const& at(Int0 i0, Int1 i1) const {
    if (std::cmp_less(i0, 0) || std::cmp_greater_equal(i0, dim0)) [[unlikely]] {
      throw std::out_of_range("argument 1 of dual_array::at is out of range");
    } else if (std::cmp_less(i1, 0) || std::cmp_greater_equal(i1, dim1))
        [[unlikely]] {
      throw std::out_of_range("argument 2 of dual_array::at is out of range");
    }
    return inside_[i0 * dim1 + i1];
  }
  std::pair<std::size_t, std::size_t> dimensions() const {
    return {dim0, dim1};
  }
  std::size_t size() const {
    return dim0 * dim1;
  }
};
namespace internal {
struct bool_ploxy {
  bool v = false;
  operator bool() const {
    return v;
  }
  bool_ploxy& operator=(bool u) {
    v = u;
    return *this;
  }
};
} // namespace internal
template <> class dual_array<bool> : public dual_array<internal::bool_ploxy> {
public:
  using dual_array<internal::bool_ploxy>::dual_array;
};
template <typename T>
internal::print_base_t& operator<<(internal::print_base_t& pb,
                                   dual_array<T> const& ar) {
  auto [H, W] = ar.dimensions();
  const auto [prefix, suffix, delim] = pb.get_range_decolater();
  const char* outer_delim = "";
  pb << prefix;
  for (auto i = 0uz; i < H; ++i) {
    pb << std::exchange(outer_delim, delim) << prefix;
    const char* inner_delim = "";
    for (auto j = 0uz; j < W; ++j) {
      pb << std::exchange(inner_delim, delim) << ar(i, j);
    }
    pb << suffix;
  }
  pb << suffix;
  return pb;
}
} // namespace common
namespace common {
template <typename T, std::size_t H, std::size_t W> class static_dual_array {
  std::array<T, W * H> inside_;

public:
  static_dual_array() : inside_{} {};
  template <std::integral Int0, std::integral Int1>
  T& operator()(Int0 i0, Int1 i1) {
    CL_ASSERT(std::cmp_greater_equal(i0, 0) && std::cmp_less(i0, H));
    CL_ASSERT(std::cmp_greater_equal(i1, 0) && std::cmp_less(i1, W));
    return inside_[i0 * W + i1];
  }
  template <std::integral Int0, std::integral Int1>
  T const& operator()(Int0 i0, Int1 i1) const {
    CL_ASSERT(std::cmp_greater_equal(i0, 0) && std::cmp_less(i0, H));
    CL_ASSERT(std::cmp_greater_equal(i1, 0) && std::cmp_less(i1, W));
    return inside_[i0 * W + i1];
  }
  template <std::integral Int0, std::integral Int1> T& at(Int0 i0, Int1 i1) {
    if (std::cmp_less(i0, 0) || std::cmp_greater_equal(i0, H)) [[unlikely]] {
      throw std::out_of_range(
          "argument 1 of static_dual_array::at is out of range");
    } else if (std::cmp_less(i1, 0) || std::cmp_greater_equal(i1, W))
        [[unlikely]] {
      throw std::out_of_range(
          "argument 2 of static_dual_array::at is out of range");
    }
    return inside_[i0 * W + i1];
  }
  template <std::integral Int0, std::integral Int1>
  T const& at(Int0 i0, Int1 i1) const {
    if (std::cmp_less(i0, 0) || std::cmp_greater_equal(i0, H)) [[unlikely]] {
      throw std::out_of_range(
          "argument 1 of static_dual_array::at is out of range");
    } else if (std::cmp_less(i1, 0) || std::cmp_greater_equal(i1, W))
        [[unlikely]] {
      throw std::out_of_range(
          "argument 2 of static_dual_array::at is out of range");
    }
    return inside_[i0 * W + i1];
  }
  constexpr std::pair<std::size_t, std::size_t> dimensions() const {
    return {H, W};
  }
  constexpr std::size_t size() const {
    return H * W;
  }
};
template <typename T, std::size_t H, std::size_t W>
internal::print_base_t& operator<<(internal::print_base_t& pb,
                                   static_dual_array<T, H, W> const& ar) {
  const auto [prefix, suffix, delim] = pb.get_range_decolater();
  const char* outer_delim = "";
  pb << prefix;
  for (auto i = 0uz; i < H; ++i) {
    pb << std::exchange(outer_delim, delim) << prefix;
    const char* inner_delim = "";
    for (auto j = 0uz; j < W; ++j) {
      pb << std::exchange(inner_delim, delim) << ar(i, j);
    }
    pb << suffix;
  }
  pb << suffix;
  return pb;
}
} // namespace common
namespace competitive {
template <typename T, std::size_t Capacity> class static_queue {
  std::array<T, Capacity> elems_;
  T* first_;
  T* last_;
  std::size_t size_;

public:
  static_queue()
      : elems_{}, first_(elems_.begin()), last_(elems_.begin()), size_() {}
  template <typename... Args> void emplace(Args&&... args) {
    CL_ASSERT(size_ < Capacity);
    *last_ = T(std::forward<Args>(args)...);
    if (++last_ == elems_.end()) {
      last_ = elems_.begin();
    }
    ++size_;
  }
  T pop() {
    CL_ASSERT(size_ > 0);
    auto ret = std::move(*first_);
    if (++first_ == elems_.end()) {
      first_ = elems_.begin();
    }
    --size_;
    return ret;
  }
  std::size_t size() const {
    return size_;
  }
};
} // namespace competitive
namespace competitive {
template <typename T, std::size_t Capacity> class static_stack {
  std::array<T, Capacity> elems_;
  std::size_t size_ = 0;

public:
  static_stack() = default;
  template <typename... Args> void emplace(Args&&... args) {
    CL_ASSERT(size_ < Capacity);
    elems_[size_++] = T(std::forward<Args>(args)...);
  }
  T pop() {
    CL_ASSERT(size_ > 0);
    return elems_[--size_];
  }
  std::size_t size() const {
    return size_;
  }
};
} // namespace competitive
namespace common {
template <std::integral Int> auto irange(Int first, Int last) {
  CL_ASSERT(std::cmp_less_equal(first, last));
  return std::views::iota(first, last);
}
template <std::integral Int> auto irange(Int last) {
  return irange(Int(0), last);
}
namespace internal {
template <std::integral Int> struct dual_irange_t {
  Int f0, f1, e0, e1;
  struct iterator {
    using difference_type = Int;
    using value_type = std::pair<Int, Int>;
    using iterator_concept = std::input_iterator_tag;
    Int f0, f1, e0, e1;
    Int v0, v1;
    auto operator*() const {
      return std::pair(v0, v1);
    }
    auto& operator++() {
      if (++v1 == e1) {
        v1 = f1;
        ++v0;
      }
      return *this;
    }
    auto operator++(int) {
      auto ret = *this;
      if (++v1 == e1) {
        v1 = f1;
        ++v0;
      }
      return ret;
    }
    bool operator==(std::default_sentinel_t) const {
      return v0 == e0;
    }
  };
  auto begin() const {
    return iterator{f0, f1, e0, e1, f0, f1};
  }
  auto end() const {
    return std::default_sentinel;
  }
};
} // namespace internal
template <std::integral Int> auto dual_irange(Int f0, Int e0, Int f1, Int e1) {
  CL_ASSERT(std::cmp_less_equal(f0, e0));
  CL_ASSERT(std::cmp_less_equal(f1, e1));
  return internal::dual_irange_t{f0, f1, e0, e1};
}
template <std::integral Int> auto dual_irange(Int e0, Int e1) {
  return dual_irange(Int(0), e0, Int(0), e1);
}
} // namespace common
namespace common {
namespace internal {
template <typename T, typename Proj, typename Comp> struct projected_compare {
  Proj proj;
  Comp comp;
  bool operator()(T const& a, T const& b) const {
    return comp(proj(a), proj(b));
  }
};
template <typename T, typename Proj, typename Comp> class priority_queue {
  using compare_t = projected_compare<T, Proj, Comp>;
  std::priority_queue<T, std::vector<T>, compare_t> queue;

public:
  priority_queue(Proj proj, Comp comp) : queue(compare_t{proj, comp}) {}
  std::size_t size() const {
    return queue.size();
  }
  T top() const {
    return queue.top();
  }
  void push(T const& v) {
    queue.push(v);
  }
  void push(T&& v) {
    queue.push(std::move(v));
  }
  template <typename... Args> void emplace(Args&&... args) {
    queue.emplace(std::forward<Args>(args)...);
  }
  void pop() {
    queue.pop();
  }
};
struct greater_t {};
struct less_t {};
template <typename T>
concept is_priority_tag =
    std::is_same_v<T, greater_t> || std::is_same_v<T, less_t>;
} // namespace internal
namespace priority {
constexpr internal::greater_t greater{};
constexpr internal::less_t less{};
} // namespace priority
template <typename T, typename Proj = std::identity>
auto make_priority_queue(internal::less_t, Proj proj = {}) {
  return internal::priority_queue<T, Proj, std::greater<>>(proj, {});
}
template <typename T, typename Proj = std::identity>
auto make_priority_queue(internal::greater_t, Proj proj = {}) {
  return internal::priority_queue<T, Proj, std::less<>>(proj, {});
}
template <typename T, typename Proj = std::identity>
auto make_priority_queue(Proj proj = {})
  requires(!internal::is_priority_tag<Proj>)
{
  return internal::priority_queue<T, Proj, std::greater<>>(proj, {});
}
} // namespace common
namespace common {
template <typename T> constexpr auto max_v = std::numeric_limits<T>::max();
template <typename T> constexpr auto min_v = std::numeric_limits<T>::min();
template <typename T> using pair = std::pair<T, T>;
} // namespace common
namespace competitive {
template <std::movable T, typename Container = std::vector<T>>
class min_max_heap {
  Container values;
  std::function<bool(T const&, T const&)> comp;
  template <bool is_less> bool compare(std::size_t i0, std::size_t i1) {
    if constexpr (is_less) {
      return comp(values[i0 - 1], values[i1 - 1]);
    } else {
      return comp(values[i1 - 1], values[i0 - 1]);
    }
  }
  bool is_min_level(std::size_t index) {
    CL_ASSERT(index > 0);
    return std::countl_zero(index) % 2 != 0;
  }
  template <bool is_min> void bubble_up(std::size_t index) {
    if (index >= 4 && compare<is_min>(index, index / 4)) {
      std::swap(values[index / 4 - 1], values[index - 1]);
      bubble_up<is_min>(index / 4);
    }
  }
  void bubble_up_init(std::size_t index) {
    if (index > 1) {
      auto parent = index / 2;
      if (is_min_level(index)) {
        if (compare<true>(parent, index)) {
          std::swap(values[parent - 1], values[index - 1]);
          bubble_up<false>(parent);
        } else {
          bubble_up<true>(index);
        }
      } else {
        if (compare<false>(parent, index)) {
          std::swap(values[parent - 1], values[index - 1]);
          bubble_up<true>(parent);
        } else {
          bubble_up<false>(index);
        }
      }
    }
  }
  template <bool is_min> void trickle_down(std::size_t i) {
    if (2 * i > values.size()) {
      return;
    }
    std::size_t t = 2 * i;
    for (std::size_t idx :
         {2 * i + 1, 4 * i, 4 * i + 1, 4 * i + 2, 4 * i + 3}) {
      if (idx > values.size()) {
        break;
      }
      if (compare<is_min>(idx, t)) {
        t = idx;
      }
    }
    if (compare<is_min>(t, i)) {
      std::swap(values[t - 1], values[i - 1]);
      if (t / 4 == i) {
        if (compare<is_min>(t / 2, t)) {
          std::swap(values[t / 2 - 1], values[t - 1]);
        }
        trickle_down<is_min>(t);
      }
    }
  }

public:
  template <typename Proj = std::identity>
  min_max_heap(Proj proj = {})
      : values{},
        comp([proj](T const& a, T const& b) { return proj(a) < proj(b); }) {}
  template <typename U> void push(U&& u) {
    values.push_back(std::forward<U>(u));
    bubble_up_init(values.size());
  }
  template <typename... Args> void emplace(Args&&... args) {
    values.emplace_back(std::forward<Args>(args)...);
    bubble_up_init(values.size());
  }
  T const& min() const {
    CL_ASSERT(values.size() > 0);
    return values[0];
  }
  T const& max() const {
    CL_ASSERT(values.size() > 0);
    if (values.size() < 3) {
      return values.back();
    } else {
      return std::max(values[1], values[2], comp);
    }
  }
  std::size_t size() const {
    return values.size();
  }
  void pop_min() {
    CL_ASSERT(size() != 0);
    std::swap(values.front(), values.back());
    values.pop_back();
    trickle_down<true>(1);
  }
  void pop_max() {
    CL_ASSERT(size() != 0);
    if (values.size() > 1) {
      std::size_t i = values.size() > 2 && compare<true>(2, 3) ? 3 : 2;
      std::swap(values[i - 1], values.back());
      values.pop_back();
      trickle_down<false>(i);
    } else {
      values.pop_back();
    }
  }
};
template <typename T, std::size_t Capacity>
using static_min_max_heap =
    min_max_heap<T, boost::container::static_vector<T, Capacity>>;
} // namespace competitive
namespace common {
template <typename T> std::vector<T> make_factorial_array(std::size_t max) {
  std::vector<T> ret(max + 1);
  ret[0] = T(1);
  for (auto i = 0uz; i < max; ++i) {
    ret[i + 1] = ret[i] * T(i + 1);
  }
  return ret;
}
} // namespace common
namespace competitive {
template <typename T, typename F> struct monoid_t {
  using value_t = T;
  T e_;
  F op_;
  constexpr monoid_t(T e, F op) : e_(e), op_(op) {}
  constexpr T operator()() const {
    return e_;
  }
  constexpr T operator()(T const& lhs, T const& rhs) const {
    return op_(lhs, rhs);
  }
};
template <typename T> constexpr monoid_t plus{T(0), std::plus<>()};
template <typename T> constexpr monoid_t multiplies{T(1), std::multiplies<>()};
template <typename T>
constexpr monoid_t min{common::max_v<T>, [](T const& a, T const& b) {
                         return std::min(a, b);
                       }};
template <typename T>
constexpr monoid_t max{common::min_v<T>, [](T const& a, T const& b) {
                         return std::max(a, b);
                       }};
} // namespace competitive
namespace common {
template <std::integral T> constexpr T div_ceil(T a, T b) {
  CL_ASSERT(std::cmp_greater(b, 0));
  if (std::cmp_less(a, 0)) {
    return a / b;
  } else {
    return (a + b - 1) / b;
  }
}
} // namespace common
namespace heuristic {
template <std::copy_constructible T, std::size_t N> class persistent_array {
  static constexpr std::size_t total_size = 512;
  static_assert(total_size % sizeof(T) == 0);
  static constexpr auto chunk_size = total_size / sizeof(T);
  static constexpr auto array_size = common::div_ceil(N, chunk_size);
  using chunk_ptr = std::shared_ptr<std::array<T, chunk_size>>;
  std::array<std::pair<std::size_t, chunk_ptr>, array_size> chunks;
  static inline std::size_t latest_version = 0;
  mutable std::size_t version;
  static constexpr auto index(std::size_t idx) {
    auto cidx = idx >> std::countr_zero(chunk_size);
    auto eidx = idx & (chunk_size - 1);
    return std::pair(cidx, eidx);
  }

public:
  persistent_array() : chunks{}, version(++latest_version) {
    for (auto& [ver, ptr] : chunks) {
      ver = version;
      ptr = std::make_shared<std::array<T, chunk_size>>();
    }
  }
  persistent_array(persistent_array const& ar)
      : chunks(ar.chunks), version(++latest_version) {
    ar.version = ++latest_version;
  }
  persistent_array& operator=(persistent_array const& ar) {
    chunks = ar.chunks;
    version = ++latest_version;
    ar.version = ++latest_version;
    return *this;
  }
  persistent_array(persistent_array&& ar)
      : chunks(std::move(chunks)), version(ar.version) {}
  persistent_array& operator=(persistent_array&& ar) {
    chunks = std::move(ar.chunks);
    version = ar.version;
    return *this;
  }
  template <typename U> void update(std::size_t idx, U&& value) {
    auto [cidx, eidx] = index(idx);
    auto& [ver, ptr] = chunks[cidx];
    if (std::exchange(ver, version) != version) {
      ptr = std::make_shared<std::array<T, chunk_size>>(*ptr);
    }
    (*ptr)[eidx] = std::forward<U>(value);
  }
  T const& get(std::size_t idx) const {
    auto [cidx, eidx] = index(idx);
    return (*chunks[cidx].second)[eidx];
  }
  struct iterator {
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using iterator_concept = std::input_iterator_tag;
    static constexpr auto end_cidx = N >> std::countr_zero(chunk_size);
    static constexpr auto end_eidx = N & (chunk_size - 1);
    persistent_array const* owner;
    std::size_t cidx, eidx;
    iterator(persistent_array const* p) : owner(p), cidx(), eidx() {}
    iterator& operator++() {
      ++eidx;
      if (eidx == chunk_size) {
        ++cidx;
        eidx = 0;
      }
      return *this;
    }
    iterator operator++(int) {
      auto ret = *this;
      operator++();
      return ret;
    }
    bool operator==(std::default_sentinel_t) const {
      return cidx == end_cidx && eidx == end_eidx;
    }
    const T& operator*() const {
      return (*owner->chunks[cidx].second)[eidx];
    }
  };
  std::size_t size() const {
    return N;
  }
  auto begin() const {
    return iterator(this);
  }
  auto end() const {
    return std::default_sentinel;
  }
};
} // namespace heuristic
namespace heuristic {
template <typename Engine> class random_engine_generator {
  static auto& get_engine() {
    thread_local Engine engine;
    return engine;
  }
  template <typename T> static auto make_distribution(T&& dist) {
    return [&engine = get_engine(), dist = std::move(dist)] mutable {
      return dist(engine);
    };
  }

public:
  template <typename T> static auto uniform_int_distribution(T min, T max) {
    return make_distribution(std::uniform_int_distribution<T>(min, max));
  }
  template <typename T> static auto uniform_int_distribution(T max) {
    return uniform_int_distribution(T(0), max);
  }
  template <typename T> static auto uniform_real_distribution(T min, T max) {
    return make_distribution(std::uniform_real_distribution<T>(min, max));
  }
  template <typename T> static auto uniform_real_distribution(T max) {
    return uniform_real_distribution<T>(0, max);
  }
  static auto bernoulli_distribution(double p = 0.5) {
    return make_distribution(std::bernoulli_distribution(p));
  }
  template <std::input_iterator Ite>
  static auto discrete_distribution(const Ite& first, const Ite& last) {
    return make_distribution(
        std::discrete_distribution<std::size_t>(first, last));
  }
  template <std::ranges::input_range Rng>
  static auto discrete_distribution(const Rng& rng) {
    return make_distribution(std::discrete_distribution<std::size_t>(
        std::ranges::begin(rng), std::ranges::end(rng)));
  }
  static auto discrete_distribution(std::initializer_list<double> list) {
    return discrete_distribution(list.begin(), list.end());
  }
  static double generate_canonical() {
    constexpr auto digits = std::numeric_limits<double>::digits;
    return std::generate_canonical<double, digits>(get_engine());
  }
  template <typename Rng> static void shuffle(Rng& rng) {
    std::ranges::shuffle(rng, get_engine());
  }
  template <std::random_access_iterator It>
  static void shuffle(It first, It last) {
    std::shuffle(first, last, get_engine());
  }
};
struct xorshift {
  std::uint64_t state;
  xorshift(std::uint64_t seed = 1) : state(seed) {}
  std::uint64_t operator()() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state ^ (state >> 32);
  }
  using result_type = std::uint64_t;
  static constexpr std::uint64_t min() {
    return 1;
  }
  static constexpr std::uint64_t max() {
    return std::numeric_limits<std::uint64_t>::max();
  }
};
} // namespace heuristic
namespace heuristic {
namespace internal {
template <typename T, std::size_t Capacity, typename Proj, typename Comp>
class static_priority_container {
  using compare_t = common::internal::projected_compare<T, Proj, Comp>;
  boost::container::static_vector<T, Capacity> cont;
  compare_t comp;

public:
  static_priority_container(Proj p, Comp c) : cont{}, comp{p, c} {}
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
  template <typename... Args> void emplace(Args&&... args) {
    push(T(std::forward<Args>(args)...));
  }
  auto begin() const {
    return cont.begin();
  }
  auto end() const {
    return cont.end();
  }
  std::size_t size() const {
    return cont.size();
  }
  void clear() {
    cont.clear();
  }
};
} // namespace internal
template <typename T, std::size_t Capacity, typename Proj = std::identity>
auto make_static_priority_container(common::internal::greater_t,
                                    Proj proj = {}) {
  return internal::static_priority_container<T, Capacity, Proj, std::greater<>>(
      proj, {});
}
template <typename T, std::size_t Capacity, typename Proj = std::identity>
auto make_static_priority_container(common::internal::less_t, Proj proj = {}) {
  return internal::static_priority_container<T, Capacity, Proj, std::less<>>(
      proj, {});
}
template <typename T, std::size_t Capacity, typename Proj = std::identity>
auto make_static_priority_container(Proj proj = {})
  requires(!common::internal::is_priority_tag<Proj>)
{
  return internal::static_priority_container<T, Capacity, Proj, std::greater<>>(
      proj, {});
}
} // namespace heuristic
namespace heuristic {
using time_t = std::chrono::milliseconds;
template <std::int64_t time_limit_ms, std::size_t update_frequency = 1>
class time_control_t {
  std::size_t update_count_ = 0, total_update_count_ = 0;
  bool enable_ = true;
  std::chrono::system_clock::time_point start_;
  time_t current_;

public:
  time_control_t() : start_(std::chrono::system_clock::now()), current_() {}
  operator bool() {
    ++total_update_count_;
    if constexpr (update_frequency != 1) {
      if (++update_count_ == update_frequency) {
        update_count_ = 0;
        current_ = std::chrono::duration_cast<time_t>(
            std::chrono::system_clock::now() - start_);
        return enable_ = (current_.count() < time_limit_ms);
      } else {
        return enable_;
      }
    } else {
      current_ = std::chrono::duration_cast<time_t>(
          std::chrono::system_clock::now() - start_);
      return (current_.count() < time_limit_ms);
    }
  }
  time_t current() const {
    return current_;
  }
  std::size_t total_update_count() const {
    return total_update_count_;
  }
};
template <std::int64_t time_limit_ms, std::int64_t start_temperature,
          std::int64_t end_temperature, std::size_t update_frequency = 1,
          typename Engine = xorshift>
class time_control_with_annealing
    : public time_control_t<time_limit_ms, update_frequency> {
  static constexpr std::size_t particle = 1 << 8;
  static constexpr std::array<double, particle> make_log_table() {
    std::array<double, particle> table;
    table[0] = -std::numeric_limits<double>::infinity();
    for (std::size_t i = 1; i < particle; ++i) {
      table[i] = std::log(double(i) / double(particle));
    }
    return table;
  }
  static constexpr std::array<double, time_limit_ms> make_temperature_table() {
    std::array<double, time_limit_ms> table;
    double T1 = end_temperature;
    double dT = double(start_temperature) / double(end_temperature);
    for (int i = 0; i < time_limit_ms; ++i) {
      double nt = i / double(time_limit_ms);
      table[i] = T1 * std::pow(dT, 1 - nt);
    }
    return table;
  }
  static constexpr auto log_table = make_log_table();
  static constexpr auto temp_table = make_temperature_table();
  std::uniform_int_distribution<std::size_t> dist;
  Engine engine;

public:
  time_control_with_annealing() : dist(0uz, particle - 1), engine() {}
  bool transition_check(double diff) {
    CL_ASSERT(this->current().count() < time_limit_ms);
    if (diff > 0) {
      return true;
    } else {
      auto p = dist(engine);
      return log_table[p] * temp_table[this->current().count()] < diff;
    }
  }
};
} // namespace heuristic
namespace common::debug {
inline void println() {
  std::cerr << std::endl;
}
template <typename... Ts> void println(Ts const&... args) {
  common::internal::print_base_t pb(std::cerr);
  pb.set_range_decolater("{", "}", ", ");
  pb.set_tuple_decolater("(", ")", ", ");
  common::internal::print<false>(pb, args...);
  std::cerr << std::endl;
}
} // namespace common::debug
#ifdef LOCAL_DEBUG
#define CL_PP_STR_I(s) #s
#define CL_PP_STR(s) CL_PP_STR_I(s)
#define DEBUG_PRINT(...)                                                       \
  common::debug::println(CL_PP_STR(__LINE__) ":" __VA_OPT__(, ) __VA_ARGS__)
#else
#define DEBUG_PRINT(...) (void)(0)
#endif
namespace common {
inline void println() {
  std::cout << "\n";
}
template <typename... Ts> void println(Ts const&... args) {
  internal::print_base_t pb(std::cout);
  internal::print<false>(pb, args...);
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
#define IGNORE_CAT_I2(a, b) a##b
#define IGNORE_CAT_I(a, b) IGNORE_CAT_I2(a, b)
#define IGNORE [[maybe_unused]] auto IGNORE_CAT_I(_ignore, __COUNTER__)

void Main() {}

int main() {
  std::cin.tie(nullptr);
  std::cin.sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(15);
  Main();
  std::cout << std::flush;
}
