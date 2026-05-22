#include "STXSCategorizer/CategorizeBDT/src/BDTClassifier.cpp"
#include "STXSCategorizer/CategorizeBDT/src/config/TrainConfig.h"
#include "STXSCategorizer/CategorizeBDT/src/parseArgsApply.cpp"
#include "STXSCategorizer/CommonUtils/interface/STXS_Categories.h"
// #include "STXSCategorizer/CommonUtils/interface/STXS_common.h"
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <RtypesCore.h>
#include <TMVA/Reader.h>
#include <TROOT.h>
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  ROOT::EnableImplicitMT();

  // Initialize arguments
  std::string weightFile, outputDir;
  std::vector<std::string> files;
  bool verbose;
  int version;

  parseArguments(argc, argv, files, weightFile, outputDir, version, verbose);

  // Initialize BDT configuration
  std::unique_ptr<BDTConfigBase> TrainConfig;
  switch (version) {
  case 0:
    TrainConfig = std::make_unique<BDTConfig<STXS_STAGE_0>>(TrainConfig0);
    break;
  case 2:
    TrainConfig =
        std::make_unique<BDTConfig<STXS_STAGE_1_2_MERGED>>(TrainConfig1p2);
    break;
  }

  BDTClassifier classifier(weightFile, TrainConfig->variables);

  // Open the files to be categorized
  ROOT::RDataFrame df("Events", files);

  // Build the vector with the BDT variables
  std::stringstream bdt_var_expr;
  bdt_var_expr << "ROOT::RVec<Float_t>{";

  bool first(true);
  for (const std::string &name : classifier.varNames) {
    if (!first)
      bdt_var_expr << ", ";
    else
      first = false;

    bdt_var_expr << "static_cast<Float_t>(" << name << ")";
  }
  bdt_var_expr << "}";

  // Classification
  auto df_classified =
      df.Define("BDT_variables", bdt_var_expr.str())
          .DefineSlot(
              "BDT_Scores",
              [&classifier](unsigned int slotIdx, const ROOT::RVecF &vars) {
                return classifier.CalculateBDTScores(slotIdx, vars);
              },
              {"BDT_variables"})
          .Define("BDT_Category",
                  [](const std::vector<Float_t> &scores) -> size_t {
                    return std::distance(
                        scores.begin(),
                        std::max_element(scores.begin(), scores.end()));
                  },
                  {"BDT_Scores"})
          .Define("HTXS_BDTIdx",
                  [&TrainConfig](Int_t htxs) {
                    return TrainConfig->getBDTCategory(htxs);
                  },
                  {TrainConfig->discriminantColumn})
          .Define(TrainConfig->classificationColumn,
                  [&TrainConfig](size_t catIdx) {
                    /* from config/Categories.h */
                    return static_cast<int>(
                        TrainConfig->getNthCategory(catIdx));
                  },
                  {"BDT_Category"});

  std::filesystem::path outputFile =
      std::filesystem::path(outputDir) / "classification.root";

  df_classified.Snapshot(
      "Events", outputFile.string(),
      {"EventWeight_lumi18", "EventWeight_lumi9", "EventWeight_lumi138",
       "EventWeight_lumi250", "EventWeight_lumi300", "EventWeight_lumi350",
       "trainWeight",

       "BDT_Category", "BDT_Scores", "HTXS_BDTIdx",
       TrainConfig->discriminantColumn, TrainConfig->classificationColumn});

  int nEvents = df_classified.Count().GetValue();
  if (verbose)
    std::cout << "Categorized " << nEvents << " in file " << outputFile.string()
              << std::endl;

  exit(EXIT_SUCCESS);
}
