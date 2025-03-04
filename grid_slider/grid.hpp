#pragma once
#include "bits/stdc++.h"
#include "boost/range/irange.hpp"

namespace debug {
template <std::size_t H, std::size_t W> class grid {
  static inline std::array<std::array<int, W>, H> inside{};
  static inline std::array<std::array<std::optional<int>, W>, H> data{};
  static inline std::array<std::array<const char*, W>, H> color{};
  static inline int count = 0;
  static inline std::optional<std::uint_least32_t> current;

  void check_data(std::size_t i, std::size_t j) {
    if (std::exchange(inside[i][j], count) != count) {
      data[i][j].reset();
      color[i][j] = "#FFFFFF";
    }
  }
  void set_both_i(std::size_t i, std::size_t j, std::optional<int> v,
                  const char* c) {
    inside[i][j] = count;
    data[i][j] = v;
    color[i][j] = c;
  }
  void set_value_i(std::size_t i, std::size_t j, std::optional<int> v) {
    check_data(i, j);
    data[i][j] = v;
  }
  void set_color_i(std::size_t i, std::size_t j, const char* c) {
    check_data(i, j);
    color[i][j] = c;
  }

public:
  grid() {
    if (current) {
      throw std::logic_error(
          std::format("There is another debug::grid that has not been "
                      "destructed (before define: {}).",
                      *current));
    }
    if (count == 0) {
      std::fstream fst("tmp/gridDataBase.csv", std::ios::out | std::ios::app);
      fst << H << "," << W << std::endl;
    }
    ++count;
    auto const& st = std::stacktrace::current(1, 1);
    current.emplace(st[0].source_line());
  }
  ~grid() {
    current.reset();
    std::fstream fst("tmp/gridDataBase.csv", std::ios::out | std::ios::app);
    for (auto i : boost::irange(H)) {
      const char* delim = "";
      for (auto j : boost::irange(W)) {
        fst << std::exchange(delim, ",");
        if (inside[i][j] == count) {
          if (data[i][j]) {
            fst << *data[i][j];
          } else {
            fst << "null";
          }
          fst << "," << color[i][j];
        } else {
          fst << "null,#FFFFFF";
        }
      }
      fst << "\n";
    }
    fst << std::endl;
  }

  template <std::integral Int0, std::integral Int1>
  void set_both(Int0 i, Int1 j, std::optional<int> v, const char* c) {
    assert(std::cmp_greater_equal(i, 0) && std::cmp_less(i, H));
    assert(std::cmp_greater_equal(j, 0) && std::cmp_less(j, W));
    set_both_i(std::size_t(i), std::size_t(j), v, c);
  }
  template <std::integral Int0, std::integral Int1>
  void set_value(Int0 i, Int1 j, std::optional<int> v) {
    assert(std::cmp_greater_equal(i, 0) && std::cmp_less(i, H));
    assert(std::cmp_greater_equal(j, 0) && std::cmp_less(j, W));
    set_value_i(std::size_t(i), std::size_t(j), v);
  }
  template <std::integral Int0, std::integral Int1>
  void set_color(Int0 i, Int1 j, const char* c) {
    assert(std::cmp_greater_equal(i, 0) && std::cmp_less(i, H));
    assert(std::cmp_greater_equal(j, 0) && std::cmp_less(j, W));
    set_color_i(std::size_t(i), std::size_t(j), c);
  }
};
} // namespace debug
