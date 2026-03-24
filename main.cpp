#include "src/STXS_Filter.cpp"
#include "src/STXS_script.cpp"
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  // STXS_script();

  if (argc < 2) {
    std::cerr << "Usage:  STXS13-plots <data_dirs>" << std::endl;
  }

  std::vector<std::string> files;
  for (int i = 1; i < argc; i++) {
    files.push_back(argv[i]);
  }

  STXS_Filter(files);
  return 0;
}
