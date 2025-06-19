#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

int main(int argc, const char* argv[]) {
  if (argc < 3 || !std::filesystem::is_regular_file(argv[1]) ||
      !std::filesystem::is_regular_file(argv[2])) {
    std::cerr << argv[0] << " <in> <out>" << std::endl;
    return 1;
  }
  constexpr std::size_t N = 10000;
  std::vector<std::pair<std::int64_t, std::int64_t>> points(N);
  std::vector<std::size_t> permutation(N - 1);
  std::vector<bool> exists(N);
  {
    std::fstream fst(argv[1]);
    std::string line;
    std::smatch match;
    std::regex regex(R"((\d+) (\d+))");
    if (!std::getline(fst, line)) {
      std::cerr << "invalid input file" << std::endl;
      return 1;
    }
    for (std::size_t i = 0; i < N; ++i) {
      if (!std::getline(fst, line)) {
        std::cerr << "invalid input file" << std::endl;
        return 1;
      }
      if (!std::regex_match(line, match, regex)) {
        std::cerr << "invalid input file" << std::endl;
        return 1;
      }
      try {
        points[i].first = std::stoll(match[1]);
        points[i].second = std::stoll(match[2]);
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
      }
    }
  }
  {
    std::fstream fst(argv[2]);
    std::string line;
    for (std::size_t i = 0; i < N - 1; ++i) {
      if (!std::getline(fst, line)) {
        std::cerr << "invalid output file" << std::endl;
        return 1;
      }
      try {
        permutation[i] = std::stoul(line);
        if (permutation[i] == 0 || permutation[i] >= N) {
          std::cerr << "out of range" << std::endl;
          return 1;
        } else if (exists[permutation[i]]) {
          std::cerr << "duplicate" << std::endl;
          return 1;
        }
        exists[permutation[i]] = true;
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
      }
    }
    std::int64_t sum = 0;
    for (std::size_t p = 0, i = 0; i < N - 1; p = permutation[i], ++i) {
      auto [a0, b0] = points[p];
      auto [a1, b1] = points[permutation[i]];
      sum += std::abs(a0 - a1) + std::abs(b0 - b1);
    }
    std::cout << "Score = " << sum << std::endl;
  }
}
