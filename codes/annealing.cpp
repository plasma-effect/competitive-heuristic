#include "bits/stdc++.h"
#include <atcoder/all>
#include <boost/multi_array.hpp>
#include <boost/range/irange.hpp>
namespace sch = std::chrono;

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
template <typename T, typename Compare = std::greater<>>
using p_queue = std::priority_queue<T, std::vector<T>, Compare>;

auto get_time() {
  static const auto start = sch::system_clock::now();
  return sch::duration_cast<sch::milliseconds>(sch::system_clock::now() -
                                               start);
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
};

} // namespace common

const int N = 26;

std::vector<int> greedy(int D, std::array<int, N> const& decrease,
                        boost::multi_array<int, 2> const& scores) {
  std::vector<int> results(D);
  std::array<int, N> last{};
  for (auto d : common::irange(D)) {
    int max = common::min_v<int>;
    int max_i = 0;
    for (auto i : common::irange(N)) {
      int score = scores[d][i];
      for (auto j : common::irange(N)) {
        if (i != j) {
          score -= decrease[j] * (d + 1 - last[j]);
        }
      }
      if (score > max) {
        max = score;
        max_i = i;
      }
    }
    results[d] = max_i;
    last[max_i] = d + 1;
  }
  return results;
}

struct get_score_t {
  int D;
  std::array<int, N> const& decrease;
  boost::multi_array<int, 2> const& scores;
  int operator()(std::vector<int> const& results) const {
    int score = 1'000'000;
    std::array<int, N> last{};
    for (auto d : common::irange(D)) {
      auto i = results[d];
      auto diff = d + 1 - last[i];
      score += scores[d][i];
      score -= diff * (diff - 1) * decrease[i] / 2;
      last[i] = d + 1;
    }
    for (auto i : common::irange(N)) {
      auto diff = D - last[i];
      score -= diff * (diff + 1) * decrease[i] / 2;
    }
    return score;
  }
};

std::vector<int> annealing(int D, std::array<int, N> const& decrease,
                           boost::multi_array<int, 2> const& scores) {
  get_score_t get_score{D, decrease, scores};
  auto results = greedy(D, decrease, scores);
  auto score = get_score(results);
  std::mt19937 engine{};
  std::uniform_int_distribution<> base{0, D - 1}, index{0, N - 1};
  std::bernoulli_distribution select;
  std::uniform_real_distribution real{0.0, 1.0};
  common::time_control_t time_control(sch::milliseconds(1900), 100);
  while (time_control) {
    auto i = base(engine);
    if (select(engine)) {
      std::uniform_int_distribution<> dist{std::max(0, i - 16),
                                           std::min(D - 1, i + 16)};
      auto j = dist(engine);
      std::swap(results[i], results[j]);
      auto s = get_score(results);
      auto threshold = time_control.annealing_threshold(s - score);
      if (s < score && threshold < real(engine)) {
        std::swap(results[i], results[j]);
      } else {
        score = s;
      }
    } else {
      auto from = results[i];
      results[i] = index(engine);
      auto s = get_score(results);
      auto threshold = time_control.annealing_threshold(s - score);
      if (s < score && threshold < real(engine)) {
        results[i] = from;
      } else {
        score = s;
      }
    }
  }
  return results;
}

void Main() {
  int D;
  std::cin >> D;
  std::array<int, N> decrease{};
  for (auto& d : decrease) {
    std::cin >> d;
  }
  boost::multi_array<int, 2> scores(boost::extents[D][N]);
  for (int i : common::irange(D)) {
    for (int j : common::irange(N)) {
      std::cin >> scores[i][j];
    }
  }
  auto results = annealing(D, decrease, scores);
  for (auto d : results) {
    std::cout << d + 1 << std::endl;
  }
}

int main() {
  std::cin.tie(nullptr);
  std::cin.sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(15);
  Main();
}
