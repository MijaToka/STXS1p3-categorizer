#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

std::string help_message() {
  std::stringstream ss;
  ss << "Usage:\n\tSTXS-Categorization-train -d DIR [-o OUTPUT_PATH -p "
        "--verbose]"
     << std::endl
     << std::endl
     << "Options:" << std::endl
     << "\t-d DIR\t\tintput directory of the MC training data." << std::endl
     << "\t-p, --preprocess\tdetermines if te data has to be preprocessed "
        "\t\t\t(calculate discriminants and reduce to only the relevant "
        "variables)"
     << std::endl
     << "\t-o, --output PATH\t\tdefines the output directory defaults to "
        "./output"
     << std::endl
     << "\t--verbose\tenables the printing out of the input specification"
     << std::endl
     << "\t-h, --help\tShow this text.";

  return ss.str();
};

enum class Flag { HELP, PREPROCESS, VERBOSE, DIRECTORY, PATH, UNKNOWN };

static const std::map<std::string, Flag> flagMap = {
    {"-h", Flag::HELP},           {"--help", Flag::HELP},
    {"-p", Flag::PREPROCESS},     {"--preprocess", Flag::PREPROCESS},
    {"--verbose", Flag::VERBOSE}, {"-d", Flag::DIRECTORY},
    {"-o", Flag::PATH},           {"--output", Flag::PATH}};

Flag getFlag(const std::string &arg) {
  auto keyvalPair = flagMap.find(arg);
  return (keyvalPair != flagMap.end() ? keyvalPair->second : Flag::UNKNOWN);
}

void parseArguments(int argc, char *argv[], std::string &directory,
                    std::string &output, bool &preprocess, bool &verbose) {

  bool hasDir(false);

  output = "./output";

  std::vector<std::string> args(argv, argv + argc);
  verbose = (std::find(args.begin(), args.end(), "--verbose") != args.end());

  if (argc < 2) {
    std::cerr << help_message();
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc; i++) {
    Flag flag = getFlag(std::string(argv[i]));
    switch (flag) {
    case Flag::VERBOSE:
      break;

    case Flag::PREPROCESS:
      preprocess = true;
      break;

    case Flag::HELP:
      std::cout << help_message();
      exit(EXIT_SUCCESS);
      break;

    case Flag::DIRECTORY: {
      if (!hasDir) {
        std::filesystem::path path(argv[++i]);
        std::string pathStr(std::filesystem::absolute(path).string());

        if (verbose)
          std::cout << "Training on data under: " << pathStr << std::endl;

        directory = pathStr;
        hasDir = true;
      }
      break;
    }

    case Flag::PATH: {
      std::filesystem::path path(argv[++i]);
      std::string pathStr(std::filesystem::absolute(path).string());
      break;
    }

    case Flag::UNKNOWN:
      std::cerr << "Error parsing arguments: Unknown flag " << argv[i]
                << std::endl;
      exit(EXIT_FAILURE);
      break;
    }
  }

  // Check the obligatory variables are set
  if (!hasDir) {
    std::cerr << "Must pass in a file." << std::endl;
    exit(EXIT_FAILURE);
  }
  if (verbose) { // Output directory verbose message
    std::stringstream ss;
    ss << "Output will be saved under " << output << "/" << std::endl;

    std::cout << ss.str() << std::endl;
  }
}
