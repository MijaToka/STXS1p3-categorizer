#include <ROOT/RDF/RDefine.hxx>
#include <ROOT/RVec.hxx>
#include <RtypesCore.h>
#include <TMVA/Reader.h>
#include <TROOT.h>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <map>
#include <memory>
#include <vector>

struct SlotReader {
  std::vector<Float_t> varBuffer;
  std::unique_ptr<TMVA::Reader> reader;
};

struct BDTClassifier {

  std::vector<SlotReader> slotReader;
  std::vector<std::string> varNames;
  BDTClassifier(std::string weightFile,
                const std::map<std::string, char> &variables) {

    for (const auto &[name, dtype] : variables) {
      varNames.push_back(name);
    }

    unsigned int nSlots =
        ROOT::IsImplicitMTEnabled() ? ROOT::GetThreadPoolSize() : 1;

    for (size_t slotIdx = 0; slotIdx < nSlots; slotIdx++) {
      SlotReader &slot = slotReader.emplace_back();

      slot.varBuffer.resize(variables.size());
      slot.reader = std::make_unique<TMVA::Reader>("");

      slot.varBuffer.resize(variables.size());
      size_t i = 0;
      for (auto const &[name, dtype] : variables) {
        slot.reader->AddVariable(name, &slot.varBuffer[i++]);
      }
      slot.reader->BookMVA("BDT STXS1p2 Muticlassifier", weightFile);
    }
  }

  std::vector<Float_t> CalculateBDTScores(unsigned int slotIdx,
                                          const ROOT::RVecF &eventVars) {
    std::copy(eventVars.begin(), eventVars.end(),
              slotReader[slotIdx].varBuffer.begin());
    return slotReader[slotIdx].reader->EvaluateMulticlass(
        "BDT STXS1p2 Muticlassifier");
  };

  int ClassifyEvent(unsigned int slotIdx, const ROOT::RVecF &eventVars) {
    // This is to be used in a RDF Define(
    // "BDT_classification", classifierInstance.ClassifyEvent,
    // {"floatVars","intVars"}
    // )
    // having defined the floatVars and intVars by bundling the varNamesX arrays

    std::copy(eventVars.begin(), eventVars.end(),
              slotReader[slotIdx].varBuffer.begin());

    auto results = slotReader[slotIdx].reader->EvaluateMulticlass(
        "BDT STXS1p2 Muticlassifier");
    return std::distance(results.begin(),
                         std::max_element(results.begin(), results.end()));
  }

  int operator()(unsigned int slotIdx, const ROOT::RVecF &eventVars) {
    return ClassifyEvent(slotIdx, eventVars);
  }
};
