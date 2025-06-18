#include "atcoder/all"
#include "bits/stdc++.h"
#include "boost/container/static_vector.hpp"
#include "boost/range/irange.hpp"
#include "debug_print.hpp"
#include "heuristic.hpp"
#include "math.hpp"
#include "normal_print.hpp"
#define IGNORE [[maybe_unused]] auto _

constexpr std::size_t N = 20;
constexpr std::size_t H = 1000;
constexpr std::size_t M = N * N;
using color_t = std::array<double, 3>;
double difference(color_t const& a, color_t const& b) {
  return std::hypot(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
color_t merge(color_t const& a, color_t const& b) {
  return color_t{
      (a[0] + b[0]) / 2,
      (a[1] + b[1]) / 2,
      (a[2] + b[2]) / 2,
  };
}
color_t merge(color_t const& a, double v, color_t const& b, double u) {
  return color_t{
      (a[0] * v + b[0] * u) / (v + u),
      (a[1] * v + b[1] * u) / (v + u),
      (a[2] * v + b[2] * u) / (v + u),
  };
}
color_t merge(std::initializer_list<color_t> colors) {
  color_t ret{};
  for (auto [C, M, Y] : colors) {
    ret[0] += C;
    ret[1] += M;
    ret[2] += Y;
  }
  ret[0] /= colors.size();
  ret[1] /= colors.size();
  ret[2] /= colors.size();
  return ret;
}

auto read_data() {
  std::size_t n, k, h, t, d;
  std::cin >> n >> k >> h >> t >> d;
  std::vector<color_t> colors(k), targets(H);
  for (auto& [c, m, y] : colors) {
    std::cin >> c >> m >> y;
  }
  for (auto& [c, m, y] : targets) {
    std::cin >> c >> m >> y;
  }
  return std::make_tuple(k, t, d, std::move(colors), std::move(targets));
}
auto make_data(std::size_t K) {
  auto dist = heuristic::make_uniform_int_distribution(100'000);
  std::vector<color_t> colors(K);
  for (auto& [C, M, Y] : colors) {
    C = dist() * 1e-5;
    M = dist() * 1e-5;
    Y = dist() * 1e-5;
  }
  std::vector<color_t> targets(H);
  for (auto& [tC, tM, tY] : targets) {
    std::vector<double> x(K);
    double xsum = 0;
    for (auto& v : x) {
      v = -std::exp(heuristic::generate_canonical());
      xsum += v;
    }
    std::vector<double> normalized;
    for (auto& v : x) {
      normalized.push_back(v / xsum);
    }
    for (auto i : common::irange(K)) {
      tC += 100'000 * normalized[i] * colors[i][0];
      tM += 100'000 * normalized[i] * colors[i][1];
      tY += 100'000 * normalized[i] * colors[i][2];
    }
    tC = std::round(tC) * 1e-5;
    tM = std::round(tM) * 1e-5;
    tY = std::round(tY) * 1e-5;
  }
  return std::make_tuple(std::move(colors), std::move(targets));
}

struct quasiliner_palette_t {
  std::array<bool, N * N - 1> partition = {};
  std::array<std::size_t, N * N> group = {};
  std::vector<std::size_t> well_sizes = {400};
  std::vector<double> ink_sizes = {0.0};
  std::vector<color_t> colors = {color_t{}};
  std::size_t D;
  std::vector<color_t> const& base;
  std::vector<color_t> const& targets;
  double score = 1;
  std::size_t next = 0;
  quasiliner_palette_t(std::size_t d, std::vector<color_t> const& b,
                       std::vector<color_t> const& t)
      : D(d), base(b), targets(t) {}

  void open_partition(std::size_t p) {
    assert(p + 1 < group.size());
    auto pgidx = group.at(p);
    auto qgidx = group.at(p + 1);
    well_sizes.at(pgidx) += well_sizes.at(qgidx);
    ink_sizes.at(pgidx) += ink_sizes.at(qgidx);
    if (ink_sizes.at(pgidx) + ink_sizes.at(qgidx) != 0) {
      colors.at(pgidx) = ::merge(colors.at(pgidx), ink_sizes.at(pgidx),
                                 colors.at(qgidx), ink_sizes.at(qgidx));
    }
    well_sizes.erase(well_sizes.begin() + qgidx);
    ink_sizes.erase(ink_sizes.begin() + qgidx);
    colors.erase(colors.begin() + qgidx);
    for (auto i : common::irange(p + 1, M)) {
      --group.at(i);
    }
  }
  void close_partition(std::size_t p) {
    assert(p < group.size());
    std::size_t before = 0, after = 0, gidx = group.at(p);
    for (auto i : common::irange(p + 1)) {
      if (group.at(i) == gidx) {
        ++before;
      }
    }
    for (auto i : common::irange(p + 1, M)) {
      if (group.at(i) == gidx) {
        ++after;
      }
      ++group.at(i);
    }
    auto ws = well_sizes.at(gidx);
    auto is = ink_sizes.at(gidx);
    auto c = colors.at(gidx);
    well_sizes.at(gidx) = before;
    well_sizes.insert(well_sizes.begin() + gidx + 1, after);
    ink_sizes.at(gidx) = before * is / ws;
    ink_sizes.insert(ink_sizes.begin() + gidx + 1, after * is / ws);
    colors.insert(colors.begin() + gidx + 1, c);
  }
  void action_add(std::size_t p, std::size_t c) {
    assert(p < group.size());
    auto gidx = group.at(p);
    auto b = std::min(1.0, well_sizes.at(gidx) - ink_sizes.at(gidx));
    colors.at(gidx) =
        ::merge(colors.at(gidx), ink_sizes.at(gidx), base.at(c), b);
    ink_sizes[gidx] += b;
    score += D;
  }
  void action_use(std::size_t p) {
    auto gidx = group.at(p);
    assert(p < group.size());
    assert(ink_sizes.at(gidx) >= 1.0 - (1e-6));
    score -= D;
    score += 10000 * ::difference(colors.at(gidx), targets.at(next++));
    ink_sizes.at(gidx) -= std::min(1.0, ink_sizes.at(gidx));
  }
  void action_remove(std::size_t p) {
    assert(p < group.size());
    auto gidx = group.at(p);
    ink_sizes.at(gidx) -= std::min(1.0, ink_sizes.at(gidx));
  }
  void set_partition(std::size_t p, std::size_t q, bool status) {
    assert(p + 1 == q);
    assert(q < N * N);
    if (status != partition.at(p)) {
      partition.at(p) = status;
      if (status) {
        close_partition(p);
      } else {
        open_partition(p);
      }
    }
  }
  double total_score() { return score; }
};

namespace lstree {
int plus(int a, int b) { return a + b; }
int max(int a, int b) { return std::max(a, b); }
int e() { return 0; }
using tree_t = atcoder::lazy_segtree<int, max, e, int, plus, plus, e>;
} // namespace lstree

constexpr std::chrono::milliseconds time_limit(1900);
double elapsed_ratio() {
  auto elapsed = static_cast<double>(heuristic::get_time().count());
  return std::min(elapsed / time_limit.count(), 1.0);
}

struct scoped_measure {
  std::chrono::milliseconds start;
  scoped_measure() : start(heuristic::get_time()) {}
  auto elapsed() const { return heuristic::get_time() - start; }
};

struct color_making_tree;
using make_tree_ptr = std::shared_ptr<color_making_tree>;
struct color_making_tree {
  virtual void make(quasiliner_palette_t& palette, std::size_t p) const = 0;
  virtual std::int64_t size() const = 0;
  virtual std::size_t total_size() const = 0;
};
struct simple_making : color_making_tree {
  make_tree_ptr prev_action;
  std::size_t c, ts;
  std::int64_t s;
  virtual void make(quasiliner_palette_t& palette,
                    std::size_t p) const override {
    if (prev_action) {
      prev_action->make(palette, p);
    }
    palette.action_add(p, c);
  }
  virtual std::int64_t size() const override { return s; }
  virtual std::size_t total_size() const override { return ts; }
  static make_tree_ptr append(make_tree_ptr prev, std::size_t c) {
    auto tree = std::make_shared<simple_making>();
    tree->prev_action = prev;
    tree->c = c;
    tree->s = prev->size() + 1;
    tree->ts = prev->total_size() + 1;
    return tree;
  }
  static make_tree_ptr create(std::size_t c) {
    auto tree = std::make_shared<simple_making>();
    tree->c = c;
    tree->s = 1;
    tree->ts = 1;
    return tree;
  }
};
struct remove_and_merge : color_making_tree {
  make_tree_ptr prev_action;
  std::size_t c, ts;
  std::int64_t s;
  virtual void make(quasiliner_palette_t& palette,
                    std::size_t p) const override {
    if (prev_action) {
      prev_action->make(palette, p);
    }
    palette.action_remove(p);
    palette.action_add(p, c);
  }
  virtual std::int64_t size() const override { return s; }
  virtual std::size_t total_size() const override { return ts; }
  static make_tree_ptr append(make_tree_ptr prev, std::size_t c) {
    assert(prev->size() >= 2);
    auto tree = std::make_shared<remove_and_merge>();
    tree->prev_action = prev;
    tree->c = c;
    tree->s = prev->size();
    tree->ts = prev->total_size() + 1;
    return tree;
  }
};
struct remove_ink : color_making_tree {
  std::size_t count;
  virtual void make(quasiliner_palette_t& palette,
                    std::size_t p) const override {
    for (IGNORE : common::irange(count)) {
      palette.action_remove(p);
    }
  }
  virtual std::int64_t size() const override {
    return -static_cast<std::int64_t>(count);
  }
  virtual std::size_t total_size() const override { return 0; }
  static make_tree_ptr create(std::size_t count) {
    auto tree = std::make_shared<remove_ink>();
    tree->count = count;
    return tree;
  }
};
struct sequence : color_making_tree {
  std::vector<make_tree_ptr> seq;
  virtual void make(quasiliner_palette_t& palette,
                    std::size_t p) const override {
    for (auto const& t : seq) {
      t->make(palette, p);
    }
  }
  virtual std::int64_t size() const override {
    std::int64_t sum = 0;
    for (auto const& t : seq) {
      sum += t->size();
    }
    return sum;
  }
  virtual std::size_t total_size() const override {
    std::size_t sum = 0;
    for (auto const& t : seq) {
      sum += t->total_size();
    }
    return sum;
  }
  static make_tree_ptr append(std::initializer_list<make_tree_ptr> list) {
    auto tree = std::make_shared<sequence>();
    tree->seq.assign(list);
    return tree;
  }
};

template <typename... Ts> auto make_cref_tuple(Ts const&... args) {
  return std::tuple<Ts const&...>(args...);
}

struct group_manager {
  std::vector<std::vector<std::size_t>> indexes = {};
  std::vector<color_t> colors = {};
  std::vector<make_tree_ptr> pre_action = {};
  std::vector<std::size_t> max_sizes = {};
  std::size_t total_max_size_ = 0;
  std::size_t total_empty_count_ = 0;
  std::vector<std::optional<std::size_t>> rindexes;
  std::span<const color_t> targets;
  std::vector<double> differences;
  std::vector<make_tree_ptr> cell_actions;
  group_manager(std::span<const color_t> t)
      : rindexes(t.size()), targets(t), differences(t.size()),
        cell_actions(t.size()) {}

  std::size_t new_group(color_t const& color, make_tree_ptr ptr) {
    auto gidx = indexes.size();
    indexes.emplace_back();
    colors.push_back(color);
    pre_action.push_back(ptr);
    max_sizes.emplace_back(ptr->size());
    total_max_size_ += max_sizes.back();
    total_empty_count_ += max_sizes.back();
    return gidx;
  }
  bool add_member(std::size_t idx, std::size_t gidx) {
    assert(gidx < indexes.size());
    assert(idx < targets.size());
    assert(indexes[gidx].size() < max_sizes[gidx]);
    if (rindexes[idx]) {
      auto pgidx = *rindexes[idx];
      auto result = std::ranges::remove(indexes[pgidx], idx);
      indexes[pgidx].erase(result.begin(), indexes[pgidx].end());
    } else {
      --total_empty_count_;
    }
    indexes[gidx].emplace_back(idx);
    std::ranges::sort(indexes[gidx]);
    rindexes[idx] = gidx;
    differences[idx] = ::difference(targets[idx], colors[gidx]);
    return true;
  }
  std::size_t size() { return indexes.size(); }
  std::size_t get_gidx(std::size_t idx) {
    assert(rindexes[idx]);
    return *rindexes[idx];
  }
  void remove_empty_group(std::size_t gidx) {
    assert(gidx < indexes.size());
    assert(indexes[gidx].size() == 0);
    total_max_size_ -= max_sizes[gidx];
    total_empty_count_ -= max_sizes[gidx];
    std::swap(indexes[gidx], indexes.back());
    indexes.pop_back();
    std::swap(colors[gidx], colors.back());
    colors.pop_back();
    std::swap(pre_action[gidx], pre_action.back());
    pre_action.pop_back();
    std::swap(max_sizes[gidx], max_sizes.back());
    max_sizes.pop_back();
    for (auto idx : indexes[gidx]) {
      rindexes[idx] = gidx;
    }
  }
  void set_cell_action(std::size_t idx, make_tree_ptr ptr) {
    assert(idx < cell_actions.size());
    cell_actions[idx] = ptr;
  }
  void make_color(quasiliner_palette_t& palette, std::size_t p,
                  std::size_t gidx) {
    assert(gidx < indexes.size());
    pre_action[gidx]->make(palette, p);
  }
  void run_cell_action(quasiliner_palette_t& palette, std::size_t p,
                       std::size_t idx) {
    assert(idx < cell_actions.size());
    if (cell_actions[idx]) {
      cell_actions[idx]->make(palette, p);
    }
  }
  std::size_t total_max_size() const { return total_max_size_; }
  std::size_t total_empty_count() const { return total_empty_count_; }

  struct sub_group {
    group_manager& manager;
    std::size_t gidx;
    sub_group(group_manager& m, std::size_t g) : manager(m), gidx(g) {}

    std::size_t empty_count() const {
      return manager.max_sizes[gidx] - manager.indexes[gidx].size();
    }
    double difference() const {
      double sum = 0;
      for (auto idx : manager.indexes[gidx]) {
        sum += manager.differences[idx];
      }
      return sum;
    }
    std::size_t size() const { return manager.indexes[gidx].size(); }
    std::size_t max_size() const { return manager.max_sizes[gidx]; }
    std::vector<std::size_t> const& members() const {
      return manager.indexes[gidx];
    }
    color_t const& color() const { return manager.colors[gidx]; }
    void color(color_t color) {
      for (auto idx : manager.indexes[gidx]) {
        manager.differences[idx] = ::difference(color, manager.targets[idx]);
      }
      manager.colors[gidx] = color;
    }
    make_tree_ptr const& get_tree() { return manager.pre_action[gidx]; }
    void set_tree(make_tree_ptr const& ptr) {
      assert(std::cmp_greater_equal(ptr->size(), manager.indexes[gidx].size()));
      manager.pre_action[gidx] = ptr;
    }
    std::size_t extra_ink_count() const {
      return manager.pre_action[gidx]->total_size() -
             manager.indexes[gidx].size();
    }
  };
  sub_group operator[](std::size_t gidx) { return sub_group(*this, gidx); }
  struct iterator {
    group_manager& manager;
    std::size_t gidx;
    iterator(group_manager& m) : manager(m), gidx() {}
    iterator& operator++() {
      ++gidx;
      return *this;
    }
    bool operator!=(std::default_sentinel_t) {
      return gidx != manager.indexes.size();
    }
    sub_group operator*() { return sub_group(manager, gidx); }
  };
  auto begin() { return iterator(*this); }
  auto end() { return std::default_sentinel; }
};

auto make_pre_merged(std::vector<color_t> const& base, std::size_t max_depth) {
  using element_t = std::tuple<make_tree_ptr, color_t>;
  scoped_measure measure;
  const auto K = base.size();
  std::vector<std::vector<element_t>> values(max_depth);
  std::vector<std::vector<std::size_t>> last(max_depth);
  for (auto i : common::irange(K)) {
    values[0].emplace_back(simple_making::create(i), base[i]);
    last[0].emplace_back(i);
  }
  for (auto d : common::irange<std::size_t>(1, max_depth)) {
    for (auto cidx : common::irange(values[d - 1].size())) {
      auto const& [tree, color] = values[d - 1][cidx];
      for (auto i : common::irange(last[d - 1][cidx], K)) {
        values[d].emplace_back(simple_making::append(tree, i),
                               ::merge(color, d, base[i], 1));
        last[d].emplace_back(i);
      }
    }
  }
  return values;
}

struct partial_optimizer {
  std::vector<color_t> const &base, targets;
  const std::size_t K, D;
  std::vector<std::vector<std::tuple<make_tree_ptr, color_t>>> pre_merged;
  partial_optimizer(std::vector<color_t> const& b,
                    std::vector<color_t> const& t, std::size_t d)
      : base(b), targets(t), K(b.size()), D(d),
        pre_merged(make_pre_merged(b, 5)) {}
  void initial_grouping(std::span<const color_t> const& vec,
                        group_manager& groups, std::size_t size) {
    scoped_measure measure;
    std::map<common::pair<std::size_t>, std::size_t> map;
    for (auto idx : common::irange(size)) {
      double min = 2;
      make_tree_ptr min_tree;
      color_t min_color{};
      common::pair<std::size_t> min_cidx{};
      for (auto len : common::irange(pre_merged.size())) {
        for (auto cidx : common::irange(pre_merged[len].size())) {
          auto const& [mtree, color] = pre_merged[len][cidx];
          auto d = difference(vec[idx], color);
          if (d <= min) {
            min = d;
            min_tree = mtree;
            min_color = color;
            min_cidx = {len, cidx};
          }
        }
      }
      if (map.contains(min_cidx)) {
        auto gidx = map[min_cidx];
        groups.add_member(idx, gidx);
        if (groups[gidx].empty_count() == 0) {
          map.erase(min_cidx);
        }
      } else {
        auto gidx = groups.new_group(min_color, min_tree);
        groups.add_member(idx, gidx);
        if (groups[gidx].empty_count() != 0) {
          map[min_cidx] = gidx;
        }
      }
    }
  }
  void reduce_groups(std::span<const color_t> const& vec,
                     group_manager& groups) {
    scoped_measure measure;
    std::priority_queue<std::tuple<double, std::size_t>> queue1;
    for (auto gidx : common::irange(groups.size())) {
      auto group = groups[gidx];
      double sum = group.empty_count() * D + 10000 * group.difference();
      queue1.emplace(sum, gidx);
    }
    std::array<bool, M> locked{};
    while (queue1.size()) {
      auto [s, gidx0] = queue1.top();
      queue1.pop();
      if (gidx0 >= groups.size() ||
          groups[gidx0].size() >
              groups.total_empty_count() - groups[gidx0].empty_count()) {
        continue;
      }
      auto const& members0 = groups[gidx0].members();
      using tuple_t = std::tuple<double, std::size_t, std::size_t>;
      common::priority_queue<tuple_t> queue2;
      for (auto gidx1 : common::irange(groups.size())) {
        if (gidx0 == gidx1 || groups[gidx1].empty_count() == 0) {
          continue;
        }
        auto color = groups[gidx1].color();
        for (auto idx : members0) {
          queue2.emplace(difference(color, vec[idx]), idx, gidx1);
        }
      }
      if (queue2.size() == 0) {
        break;
      }
      for (auto idx : members0) {
        locked[idx] = false;
      }
      std::size_t rest = members0.size();
      while (queue2.size()) {
        auto [_, idx, gidx1] = queue2.top();
        queue2.pop();
        if (locked[idx] || groups[gidx1].empty_count() == 0) {
          continue;
        }
        locked[idx] = true;
        assert(groups.add_member(idx, gidx1));
        if (--rest == 0) {
          break;
        }
      }
      groups.remove_empty_group(gidx0);
      if (gidx0 != groups.size()) {
        auto sum = groups[gidx0].empty_count() * D +
                   10000 * groups[gidx0].difference();
        queue1.emplace(sum, gidx0);
      }
    }
  }
  void optimize_color(std::span<const color_t> vec, group_manager& groups) {
    scoped_measure measure;
    for (auto group : groups) {
      double min = 2 * group.size();
      make_tree_ptr min_tree;
      color_t min_color{};
      for (auto const& [tree, color] : pre_merged[group.size() - 1]) {
        double sum = 0;
        for (auto idx : group.members()) {
          sum += ::difference(vec[idx], color);
        }
        if (sum < min) {
          min = sum;
          min_tree = tree;
          min_color = color;
        }
      }
      group.color(min_color);
      group.set_tree(min_tree);
    }
  }
  void append_color(std::span<const color_t> const& vec, group_manager& groups,
                    heuristic::time_control_t& control, std::size_t& turn,
                    std::size_t max_turn) {
    scoped_measure measure;
    using element_t = std::tuple<double, double, std::size_t, std::size_t>;
    auto get_min_append = [&base = this->base,
                           &vec](group_manager::sub_group const& group) {
      double min = 1e+9;
      auto color = group.color();
      std::size_t min_c = 0;
      for (auto i : common::irange(base.size())) {
        auto nc = ::merge(color, group.size() - 1, base[i], 1);
        double new_score = 0;
        for (auto idx : group.members()) {
          new_score += 10000 * difference(nc, vec[idx]);
        }
        if (new_score < min) {
          min = new_score;
          min_c = i;
        }
      }
      return std::make_tuple(min, min_c);
    };
    std::priority_queue<element_t> queue;
    for (auto gidx : common::irange(groups.size())) {
      auto group = groups[gidx];
      if (group.size() < 2) {
        continue;
      }
      double ink_score = group.extra_ink_count() * D;
      auto score = ink_score + 10000 * group.difference();
      auto [min, min_c] = get_min_append(group);
      min += ink_score + D;
      if (min < score) {
        queue.emplace(score - min, min, gidx, min_c);
      }
    }
    while (queue.size() && control) {
      auto [_, score, gidx, c] = queue.top();
      queue.pop();
      auto group = groups[gidx];
      auto tree = group.get_tree();
      auto nc = merge(group.color(), group.size() - 1, base[c], 1);
      tree = remove_and_merge::append(tree, c);
      group.set_tree(tree);
      group.color(nc);
      turn += 2;
      if (turn == max_turn) {
        break;
      }
      double ink_score = group.extra_ink_count() * D;
      auto [min, min_c] = get_min_append(group);
      min += ink_score + D;
      if (min < score) {
        queue.emplace(score - min, min, gidx, min_c);
      }
    }
  }
  void output(group_manager& groups, quasiliner_palette_t& palette,
              std::size_t size) {
    scoped_measure measure;
    std::vector<std::size_t> group_location(groups.size());
    for (std::size_t shift = 0; auto gidx : common::irange(groups.size())) {
      if (shift != 0) {
        palette.set_partition(shift - 1, shift, true);
      }
      auto len = groups[gidx].size();
      for (auto i : common::irange<std::size_t>(1, len)) {
        palette.set_partition(i + shift - 1, i + shift, false);
      }
      group_location[gidx] = shift;
      shift += len;
    }
    for (auto gidx : common::irange(groups.size())) {
      groups.make_color(palette, group_location[gidx], gidx);
    }
    double max = 0, sum = 0;
    [[maybe_unused]] std::size_t max_idx = 0;
    for (auto idx : common::irange(size)) {
      auto gidx = groups.get_gidx(idx);
      auto d = groups.differences[idx];
      if (d > max) {
        max = d;
        max_idx = idx;
      }
      sum += d;
      groups.run_cell_action(palette, group_location[gidx], idx);
      palette.action_use(group_location[gidx]);
    }
  }
  void operator()(quasiliner_palette_t& palette, std::size_t first_idx,
                  std::size_t last_idx, [[maybe_unused]] std::size_t max_turn,
                  [[maybe_unused]] std::chrono::milliseconds time_limit) {
    const auto size = last_idx - first_idx;
    std::span vec(targets.begin() + first_idx, size);
    group_manager groups(vec);
    [[maybe_unused]] heuristic::time_control_t control(time_limit);

    initial_grouping(vec, groups, size);
    reduce_groups(vec, groups);
    optimize_color(vec, groups);
    std::size_t turn = 2 * size;
    append_color(vec, groups, control, turn, max_turn);
    output(groups, palette, size);
  }
};

struct order_optimzer {
  std::vector<color_t> const &base, targets;
  const std::size_t K, D;
  std::vector<std::vector<std::tuple<make_tree_ptr, color_t>>> pre_merged;
  order_optimzer(std::vector<color_t> const& b, std::vector<color_t> const& t,
                 std::size_t d)
      : base(b), targets(t), K(b.size()), D(d),
        pre_merged(make_pre_merged(b, b.size() < 15 ? 5 : 4)) {}
  void output(group_manager& groups, quasiliner_palette_t& palette,
              std::size_t size) {
    scoped_measure measure;
    std::vector<std::size_t> group_location(groups.size());
    for (std::size_t shift = 0; auto gidx : common::irange(groups.size())) {
      if (shift != 0) {
        palette.set_partition(shift - 1, shift, true);
      }
      auto len = groups[gidx].size();
      for (auto i : common::irange<std::size_t>(1, len)) {
        palette.set_partition(i + shift - 1, i + shift, false);
      }
      group_location[gidx] = shift;
      shift += len;
    }
    for (auto gidx : common::irange(groups.size())) {
      groups.make_color(palette, group_location[gidx], gidx);
    }
    for (auto idx : common::irange(size)) {
      auto gidx = groups.get_gidx(idx);
      groups.run_cell_action(palette, group_location[gidx], idx);
      palette.action_use(group_location[gidx]);
    }
  }
  void operator()(quasiliner_palette_t& palette, std::size_t first_idx,
                  std::size_t last_idx, [[maybe_unused]] std::size_t max_turn,
                  [[maybe_unused]] std::chrono::milliseconds time_limit) {
    const auto size = last_idx - first_idx;
    std::span vec(targets.begin() + first_idx, size);
    group_manager groups(vec);
    std::vector<color_t> last_color;
    std::vector<std::size_t> ink_count;
    {
      scoped_measure measure;
      double min = 2;
      make_tree_ptr min_tree;
      color_t min_color{};
      for (auto d : common::irange(pre_merged.size())) {
        for (auto const& [tree, color] : pre_merged[d]) {
          auto diff = ::difference(vec[0], color);
          if (diff < min) {
            min = diff;
            min_tree = tree;
            min_color = color;
          }
        }
      }
      auto gidx = groups.new_group(min_color, min_tree);
      groups.add_member(0, gidx);
      last_color.emplace_back(min_color);
      ink_count.emplace_back(groups[gidx].max_size() - 1);
    }
    {
      scoped_measure measure;
      auto turn = max_turn - 2 * size;
      for (auto idx : common::irange<std::size_t>(1, size)) {
        double min = 20000;
        make_tree_ptr min_tree;
        color_t min_color{};
        std::size_t min_gidx = groups.size();
        std::size_t min_remove_count = 0;
        for (auto d : common::irange(pre_merged.size())) {
          if (d + 1 + groups.total_max_size() > size) {
            break;
          }
          for (auto const& [tree, color] : pre_merged[d]) {
            auto diff = 10000 * ::difference(vec[idx], color);
            if (diff < min) {
              min = diff;
              min_tree = tree;
              min_color = color;
            }
          }
        }
        for (auto gidx : common::irange(groups.size())) {
          if (groups[gidx].empty_count() == 0) {
            continue;
          }
          if (auto score = 10000 * ::difference(last_color[gidx], vec[idx]);
              score < min) {
            min = score;
            min_tree = nullptr;
            min_color = last_color[gidx];
            min_gidx = gidx;
          }
          for (auto d : common::irange(groups[gidx].max_size() - 1)) {
            std::size_t count =
                d + 1 + ink_count[gidx] > groups[gidx].max_size()
                    ? d + 1 + ink_count[gidx] - groups[gidx].max_size()
                    : 0;
            if (d + 1 + count > turn) {
              break;
            }
            for (auto const& [tree, color] : pre_merged[d]) {
              auto nc = ::merge(last_color[gidx], ink_count[gidx] - count,
                                color, tree->size());
              if (auto score = (d + 1) * D + 10000 * ::difference(nc, vec[idx]);
                  score < min) {
                min = score;
                if (count == 0) {
                  min_tree = tree;
                } else {
                  min_tree =
                      sequence::append({remove_ink::create(count), tree});
                }
                min_color = nc;
                min_remove_count = count;
                min_gidx = gidx;
              }
            }
          }
        }
        if (min_gidx == groups.size()) {
          assert(min_tree);
          groups.new_group(min_color, min_tree);
          groups.add_member(idx, min_gidx);
          last_color.emplace_back(min_color);
          ink_count.emplace_back(groups[min_gidx].max_size() - 1);
        } else {
          groups.add_member(idx, min_gidx);
          groups.set_cell_action(idx, min_tree);
          last_color[min_gidx] = min_color;
          if (min_tree) {
            ink_count[min_gidx] = groups[min_gidx].max_size();
            turn -= min_tree->total_size() + min_remove_count;
          }
          --ink_count[min_gidx];
        }
      }
    }
    output(groups, palette, size);
  }
};

template <typename Optimizer>
double execute(std::vector<color_t> const& colors,
               std::vector<color_t> const& targets, std::size_t T,
               std::size_t D) {
  using namespace std::chrono_literals;
  Optimizer optimizer(colors, targets, D);
  quasiliner_palette_t palette(D, colors, targets);
  const auto turn = T - 3 * M;
  optimizer(palette, 0, 400, turn * 2 / 5, 1100ms);

  for (auto p : common::irange(N * N)) {
    palette.action_remove(p);
  }
  optimizer(palette, 400, 800, turn * 2 / 5, 2300ms);

  palette.set_partition(M / 2 - 1, M / 2, true);
  for (auto p : common::irange(M / 2)) {
    palette.action_remove(p);
  }
  optimizer(palette, 800, 1000, turn / 5, 2900ms);

  return palette.total_score();
}

double get_ratio(std::vector<color_t> const& colors,
                 std::vector<color_t> const& targets, std::size_t T,
                 std::size_t D) {
  return execute<order_optimzer>(colors, targets, T, D) /
         execute<partial_optimizer>(colors, targets, T, D);
}

void Main() {
  std::vector<std::future<void>> vec;
  std::vector<std::size_t> threshold(21);
  std::binary_semaphore guard{1};
  for (std::size_t K : common::irange(4, 21)) {
    vec.push_back(std::async(std::launch::async, [&guard, &threshold, K] {
      constexpr std::size_t count = 16, T = 16000;
      std::size_t min = 10, max = 1000;
      while (max - min > 1) {
        auto mid = std::midpoint(min, max);
        double sum = 0;
        for (IGNORE : common::irange(count)) {
          auto [colors, targets] = make_data(K);
          sum += get_ratio(colors, targets, T, mid);
        }
        double result = sum / count;
        guard.acquire();
        DEBUG_PRINT(K, min, max, mid, result);
        guard.release();
        if (result < 1.0) {
          min = mid;
        } else {
          max = mid;
        }
      }
      threshold[K] = max;
      guard.acquire();
      DEBUG_PRINT("end execution K =", K);
      guard.release();
    }));
  }
  for (auto& v : vec) {
    v.wait();
  }
  for (auto i : common::irange(4, 21)) {
    common::println(i, threshold[i]);
  }
}

int main() {
  std::cin.tie(nullptr);
  std::cin.sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(15);
  Main();
  std::cout << std::flush;
}
