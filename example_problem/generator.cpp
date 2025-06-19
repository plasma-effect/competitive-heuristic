#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <regex>
#include <set>

int main(int argc, const char* argv[]) {
  std::string seedpath, outdir = "in";
  std::regex reg(R"(--dir=(\w+))");
  for (int i = 1; i < argc; ++i) {
    std::cmatch match;
    if (std::regex_match(argv[i], match, reg)) {
      outdir = match[1];
    } else {
      seedpath = argv[i];
    }
  }
  if (!std::filesystem::is_regular_file(seedpath)) {
    std::cerr << "no such file: " << seedpath << std::endl;
    return 1;
  }
  std::filesystem::create_directories(outdir);

  std::fstream seed(seedpath);
  for (std::string line; std::getline(seed, line);) {
    std::uint64_t s;
    try {
      s = std::stoull(line);
    } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }
    std::mt19937 engine(s);
    std::set<std::pair<int, int>> set;
    std::uniform_int_distribution<int> dist(0, 1000);
    auto filename = std::format("{}/{:04}.txt", outdir, s);
    std::fstream fst(filename, std::ios::out | std::ios::trunc);
    constexpr std::size_t N = 10000;
    fst << N << "\n";
    for (std::size_t i = 0; i < N; ++i) {
      std::pair<int, int> p;
      do {
        p.first = dist(engine);
        p.second = dist(engine);
      } while (set.contains(p));
      set.emplace(p);
      fst << p.first << " " << p.second << "\n";
    }
  }
}
