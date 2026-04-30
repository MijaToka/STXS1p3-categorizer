#include <TMVA/Reader.h>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <map>
#include <vector>

struct BDTClassifier {

  TMVA::Reader *reader;

  std::vector<float> varBuffer;
  std::vector<std::string> varNames;

  BDTClassifier(std::string weightFile,
                const std::map<std::string, char> &variables) {
    reader = new TMVA::Reader("");

    varBuffer.resize(variables.size());
    size_t i = 0;
    for (auto const &[name, dtype] : variables) {
      varNames.push_back(name);
      reader->AddVariable(name, &varBuffer[i++]);
    }
    reader->BookMVA("BDT STXS1p2 Muticlassifier", weightFile);
  }

  template <typename... Args> int operator()(Args... args) {
    // This is to be used in a RDF Define(
    // "BDT_classification", classifierInstance, classifierInstance.varNames
    // )

    size_t i = 0;
    ((varBuffer[i++] = static_cast<float>(args)),
     ...); // Safeguard for int valued branches

    auto results = reader->EvaluateMulticlass("BDT STXS1p2 Muticlassifier");
    return std::distance(results.begin(),
                         std::max_element(results.begin(), results.end()));
  }
};
