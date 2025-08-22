#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;

int main(int argc, const char* argv[]) {
  if (argc < 3) {
    std::cerr << argv[0] << " <setting.json> <sum of scores>" << std::endl;
    return EXIT_FAILURE;
  }
  if (!fs::is_regular_file(fs::status(argv[1]))) {
    std::cerr << "ERROR: " << argv[1]
              << " doesn't exist or is not regular file." << std::endl;
    return EXIT_FAILURE;
  }
  std::fstream fst(argv[1]);
  std::int64_t score = std::stoll(argv[2]);
  auto setting = nlohmann::json::parse(fst);
  bool increase = setting["increase"].get<bool>();
  std::string result = setting["result"].get<std::string>();
  std::int64_t best = increase ? 0 : 100'000'000'000;
  if (fs::exists(fs::status(result))) {
    std::fstream best_result(result);
    best_result >> best;
  }
  if ((increase && score > best) || (!increase && score < best)) {
    std::cout << "update the best score by " << std::abs(best - score);
    if (best != 0 && score != 0) {
      std::cout << std::fixed << std::setprecision(3);
      double ratio = 100 * double(score) / double(best);
      std::cout << " (" << ratio << "%)" << std::endl;
    } else {
      std::cout << std::endl;
    }
    std::cout << "new best score is " << score << std::endl;
    std::fstream best_result(result, std::ios::out);
    best_result << score << std::endl;
  } else {
    std::cout << score << " (best score = " << best
              << ", diff = " << std::abs(best - score);
    if (best != 0 && score != 0) {
      std::cout << std::fixed << std::setprecision(3);
      double ratio = 100 * double(score) / double(best);
      std::cout << ", " << ratio << "%)" << std::endl;
    } else {
      std::cout << ")" << std::endl;
    }
  }
}
