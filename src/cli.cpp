#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <sysexits.h>

const std::string PROGRAM = "zort";

const int MINIMUM_ARGUMENT_COUNT = 2;

void write_usage();

std::vector<std::string_view> parse_file_paths(int argc, char *argv[]);

bool check_file_paths_exist(const std::vector<std::string_view> &file_paths);

int main(int argc, char *argv[]) {
  if (argc < MINIMUM_ARGUMENT_COUNT) {
    write_usage();
    return EX_USAGE;
  }

  const auto file_paths = parse_file_paths(argc, argv);
  if (file_paths.empty()) {
    write_usage();
    return EX_USAGE;
  }

  if (!check_file_paths_exist(file_paths)) {
    return EX_NOINPUT;
  }

  return EX_OK;
}

void write_usage() {
  std::cerr << "Usage: " << PROGRAM << " [file ...]" << std::endl;
}

std::vector<std::string_view> parse_file_paths(int argc, char *argv[]) {
  std::vector<std::string_view> file_paths;

  for (int i = 1; i < argc; ++i) {
    std::string_view argument(argv[i]);
    // TODO: validate file path arguments
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
      std::cerr << PROGRAM << ": No such file or directory (" << file_path << ")" << std::endl;
      return false;
    }
  }

  return true;
}
