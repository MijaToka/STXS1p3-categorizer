#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

std::string help_message() {
  std::stringstream ss;
  ss << "Usage:\n\tSTXS-Categorization-apply -f FILE [-f FILE -o OUTPUT_PATH "
        "-w WEIGHTS --verbose]"
     << std::endl
     << std::endl
     << "Options:" << std::endl
     << "\t-f, --file FILE\t\tintput file to be categorized (can be passed "
        "multiple "
        "times)."
     << std::endl
     << "\t-w, --weights WEIGHTS\tpath to the weight files produced by the "
        "training."
     << std::endl
     << "\t-o, --output PATH\t\tdefines the output directory defaults to "
        "./output"
     << std::endl
     << "\t--verbose\tenables the printing out of the input specification"
     << std::endl
     << "\t-h, --help\tShow this text.";

  return ss.str();
};

enum class Flag { HELP, VERBOSE, FILE, OUTPUT, WEIGHTS, UNKNOWN };

static const std::map<std::string, Flag> flagMap = {
    {"-h", Flag::HELP},           {"--help", Flag::HELP},
    {"-f", Flag::FILE},           {"--file", Flag::FILE},
    {"--verbose", Flag::VERBOSE}, {"-w", Flag::WEIGHTS},
    {"--weights", Flag::WEIGHTS}, {"-o", Flag::OUTPUT},
    {"--output", Flag::OUTPUT}};

Flag getFlag(const std::string &arg) {
  auto keyvalPair = flagMap.find(arg);
  return (keyvalPair != flagMap.end() ? keyvalPair->second : Flag::UNKNOWN);
}

void parseArguments(int argc, char *argv[], std::vector<std::string> &files,
                    std::string &weightFile, std::string &output,
                    bool &verbose) {

  bool hasFiles(false), hasWeights(false);

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

    case Flag::HELP:
      std::cout << help_message();
      exit(EXIT_SUCCESS);
      break;

    case Flag::FILE: {
      std::filesystem::path path(argv[++i]);
      std::string pathStr(std::filesystem::absolute(path).string());

      if (verbose)
        std::cout << "Adding file: " << pathStr << std::endl;

      files.push_back(pathStr);
      hasFiles = true;
      break;
    }

    case Flag::WEIGHTS: {
      std::filesystem::path path(argv[++i]);
      std::string pathStr(std::filesystem::absolute(path).string());

      if (verbose)
        std::cout << "Reading weight file: " << pathStr << std::endl;

      weightFile = pathStr;
      hasWeights = true;
      break;
    }
    case Flag::OUTPUT: {
      std::filesystem::path path(argv[++i]);
      output = std::filesystem::absolute(path).string();
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
  if (!hasFiles) {
    std::cerr << "Must pass in a file." << std::endl;
    exit(EXIT_FAILURE);
  }
  if (!hasWeights) {
    std::cerr << "Must pass in the weights file." << std::endl;
    exit(EXIT_FAILURE);
  }
  if (verbose) { // Output directory verbose message
    std::stringstream ss;
    ss << "Output will be saved under " << output << "/" << std::endl;

    std::cout << ss.str() << std::endl;
  }
}
