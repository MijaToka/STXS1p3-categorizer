#include "interface/STXS_common.h"
#include "src/Preprocess.cpp"
#include "src/STXS_categorization0.cpp"
#include "src/STXS_categorization1p2.cpp"
#include "src/STXS_categorization1p3.cpp"
#include "src/STXS_script.cpp"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::string help_message() {
  std::stringstream ss;
  ss << "Usage:\n\tSTXS13-plots -f ROOT_FILE [-f ROOT_FILE]... -v VERSION"
     << std::endl
     << std::endl;
  ss << "Options:" << std::endl;
  ss << "\t-f FILE\tintput file; may be given multiple times" << std::endl;
  ss << "\t-v VERSION\t version number: 2 or 3" << std::endl;
  ss << "\t-h, --help\t Show this text.";

  return ss.str();
};

void parseArguments(
    int argc, char *argv[], std::vector<std::string> &files,
    std::function<std::map<STXS1, ROOT::RDF::RNode>(
        std::map<STXS0, ROOT::RDF::RNode>)> &second_categorization) {
  if (argc < 3) {
    std::cerr << help_message();
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
      std::cout << help_message();
      exit(EXIT_SUCCESS);
    }

    if (std::string(argv[i]) == "-f")
      files.push_back(argv[++i]);

    else if (std::string(argv[i]) == "-v")
      try {
        int version = std::stoi(argv[++i]);
        if (!(version == 2 || version == 3))
          throw std::invalid_argument("must be 2 or 3");
        else if (version == 2)
          second_categorization = Version1_2::second_categorization;
        else if (version == 3)
          second_categorization = Version1_3::second_categorization;
        std::cout << "Running version: " << version << std::endl;
      } catch (const std::exception &e) {
        std::cerr << "Invalid -v value: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
      }
    else {
      std::cerr << "Error parsing arguments" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char *argv[]) {

  std::function<std::map<STXS1, ROOT::RDF::RNode>(
      std::map<STXS0, ROOT::RDF::RNode>)>
      second_categorization;
  std::vector<std::string> files;

  parseArguments(argc, argv, files, second_categorization);

  auto df = setup(files);

  snapshot(df, "output");

  auto step0 = first_categorization(df);

  snapshot(step0, "output");

  auto step1 = second_categorization(step0);

  snapshot(step1, "output");

  exit(EXIT_SUCCESS);
}
