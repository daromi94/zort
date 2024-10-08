#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <sysexits.h>

#define PROGRAM "zort"

namespace zort {

const int MINIMUM_ARGUMENT_COUNT = 2;

void write_usage() {
  std::cerr << "usage: " << PROGRAM << " [file ...]" << std::endl;
}

std::vector<std::string_view> parse_file_paths(const int argc, char *argv[]) {
  std::vector<std::string_view> file_paths;

  for (int i = 1; i < argc; ++i) {
    const std::string_view argument(argv[i]);
    if (argument.starts_with('-')) {
      break;
    }
    file_paths.emplace_back(argument);
  }

  return file_paths;
}

bool check_file_paths_exist(const std::vector<std::string_view> &file_paths) {
  for (const auto &file_path : file_paths) {
    if (!std::filesystem::exists(file_path)) {
      std::cerr << PROGRAM << ": No such file (" << file_path << ")" << std::endl;
      return false;
    }
  }

  return true;
}

std::tuple<bool, std::vector<std::string>> read_lines(const std::vector<std::string_view> &file_paths) {
  std::vector<std::string> lines;

  for (const auto &file_path : file_paths) {
    std::ifstream file(file_path);
    if (!file) {
      std::cerr << PROGRAM << ": Unable to open file (" << file_path << ")" << std::endl;
      return {false, lines};
    }

    std::string line;
    while (std::getline(file, line)) {
      lines.emplace_back(line);
    }

    file.close();
  }

  return {true, lines};
}
}

int main(int argc, char *argv[]) {
  if (argc < zort::MINIMUM_ARGUMENT_COUNT) {
    zort::write_usage();
    return EX_USAGE;
  }

  const auto file_paths = zort::parse_file_paths(argc, argv);
  if (file_paths.empty()) {
    zort::write_usage();
    return EX_USAGE;
  }

  if (!zort::check_file_paths_exist(file_paths)) {
    return EX_NOINPUT;
  }

  const auto [lines_ok, lines] = zort::read_lines(file_paths);
  if (!lines_ok) {
    return EX_IOERR;
  }

  if (lines.empty()) {
    std::clog << PROGRAM << ": No lines to process" << std::endl;
    return EX_OK;
  }

  for (const auto &line : lines) {
    std::cout << line << std::endl;
  }

  return EX_OK;
}
