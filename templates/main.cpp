#include "atcoder/all"
#include "bits/stdc++.h"
#include "boost/container/static_vector.hpp"
#include "boost/range/irange.hpp"
#include "debug_print.hpp"
#include "heuristic.hpp"
#include "math.hpp"
#include "normal_print.hpp"
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
