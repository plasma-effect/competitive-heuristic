#include <cassert>
#include <format>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>

int main() {
  using namespace std::literals;
  std::string line;
  if (!std::getline(std::cin, line)) {
    std::cout << "const gridData = [];" << std::endl;
    return EXIT_SUCCESS;
  }
  int H = -1, W = -1;
  for (auto view : std::string_view(line) | std::views::split(","sv)) {
    if (H == -1) {
      H = std::stoi(std::string(view.begin(), view.end()));
    } else {
      W = std::stoi(std::string(view.begin(), view.end()));
    }
  }
  assert(H != -1);
  assert(W != -1);
  std::cout << "const gridData = [";
  int i = 0, j = 0, c = 0;
  std::string value;
  const char* delim0 = "";
  const char* delim1 = "";
  while (std::getline(std::cin, line)) {
    for (auto view : std::string_view(line) | std::views::split(","sv)) {
      if (std::exchange(c, 1 - c) == 0) {
        value = std::string(view.begin(), view.end());
      } else {
        if (i == 0 && j == 0) {
          std::cout << std::format(
              "{}{{ \"rows\": {}, \"cols\": {}, \"cells\": [\n",
              std::exchange(delim0, ","), H, W);
        }
        std::string color{view.begin(), view.end()};
        std::cout << std::format("{}{{ \"num\": {}, \"color\": \"{}\" }}\n",
                                 std::exchange(delim1, ","), value, color);
        if (++j == W) {
          j = 0;
          if (++i == H) {
            i = 0;
            std::cout << "]}\n";
            delim1 = "";
          }
        }
      }
    }
  }
  std::cout << "];" << std::endl;
}
