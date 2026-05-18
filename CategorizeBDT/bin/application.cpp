#include "STXSCategorizer/CategorizeBDT/src/BDTClassifier.cpp"
#include "STXSCategorizer/CategorizeBDT/src/config/Categories.h"
#include "STXSCategorizer/CategorizeBDT/src/config/Variables.h"
#include "STXSCategorizer/CategorizeBDT/src/parseArgsApply.cpp"
#include "STXSCategorizer/CommonUtils/interface/STXS_Categories.h"
// #include "STXSCategorizer/CommonUtils/interface/STXS_common.h"
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <TMVA/Reader.h>
#include <TROOT.h>
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iterator>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  ROOT::EnableImplicitMT();

  std::string weightFile, outputDir;
  std::vector<std::string> files;
  bool verbose;

  parseArguments(argc, argv, files, weightFile, outputDir, verbose);

  ROOT::RDataFrame df("Events", files);

  BDTClassifier classifier(weightFile, variablesSTXS1p2);

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
          .Define(
              "BDT_stage1_2_cat_pTjet30GeV_merged",
              [](size_t catIdx) {
                /* from config/Categories.h */
                return (catIdx < STXS1p2TrainConfig::TrainedCategories.size())
                           ? static_cast<int>(
                                 STXS1p2TrainConfig::TrainedCategories[catIdx])
                           : static_cast<int>(STXS_STAGE_1_2_MERGED::UNKNOWN);
              },
              {"BDT_Category"});

  size_t i = 0;
  std::filesystem::path outputFile =
      std::filesystem::path(outputDir) / "classification.root";
  df_classified.Snapshot(
      "Events", outputFile.string(),
      {"EventWeight_lumi18", "EventWeight_lumi9", "EventWeight_lumi138",
       "EventWeight_lumi250", "EventWeight_lumi300", "EventWeight_lumi350",

       "BDT_Category", "BDT_Scores", "HTXS_stage1_2_cat_pTjet30GeV_merged",
       "BDT_stage1_2_cat_pTjet30GeV_merged"});

  int nEvents = df_classified.Count().GetValue();
  if (verbose)
    std::cout << "Categorized " << nEvents << " in file " << outputFile.string()
              << std::endl;

  exit(EXIT_SUCCESS);
}
