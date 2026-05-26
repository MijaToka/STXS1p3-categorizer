
#include "STXSCategorizer/CategorizeBDT/src/config/Categories.h"
#include "STXSCategorizer/CommonUtils/interface/STXS_Categories.h"
#include <ROOT/RDataFrame.hxx>
#include <TDictionary.h>
#include <TMVA/RBDT.hxx>
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
int main(int argc, char *argv[]) {

  std::vector<std::string> files = {"ourput/train0/ggH/snapshot.root"};
  TMVA::Experimental::RBDT bdt("multiclassXGB", "multiclassXGB_model.root");
  ROOT::RDataFrame df("Events", files);
  const std::vector<std::string> features = {"DVBF1j_ME",
                                             "DVBF2j_ME",
                                             "DWHh_ME",
                                             "DZHh_ME",
                                             "JetLeading_btag",
                                             "JetLeading_eta",
                                             "JetLeading_mass",
                                             "JetLeading_phi",
                                             "JetLeading_pt",
                                             "JetSubleading_btag",
                                             "JetSubleading_eta",
                                             "JetSubleading_mass",
                                             "JetSubleading_phi",
                                             "JetSubleading_pt",
                                             "LepEta_0",
                                             "LepEta_1",
                                             "LepEta_2",
                                             "LepEta_3",
                                             "LepEta_4",
                                             "LepEta_5",
                                             "LepPhi_0",
                                             "LepPhi_1",
                                             "LepPhi_2",
                                             "LepPhi_3",
                                             "LepPt_0",
                                             "LepPt_1",
                                             "LepPt_2",
                                             "LepPt_3",
                                             "LepPt_4",
                                             "LepPt_5",
                                             "PFMET_pt",
                                             "ZZCand_KD_bestCand",
                                             "ZZCand_Phi1_bestCand",
                                             "ZZCand_costheta1_bestCand",
                                             "ZZCand_costheta2_bestCand",
                                             "ZZCand_costhetastar_bestCand",
                                             "ZZCand_eta_bestCand",
                                             "ZZCand_mass_bestCand",
                                             "ZZCand_nExtraLep_bestCand",
                                             "ZZCand_phi_bestCand",
                                             "ZZCand_pt_bestCand",
                                             "deltaEta_jj",
                                             "deltaPhi_jj",
                                             "m_jj",
                                             "nBtagged_filtered",
                                             "nCleanedJetsPt30"};
  std::vector<STXS_STAGE_0> STXS_Category;
  for (auto &[cat, name] : STXSTrainCategories::CategoriesToTrainStage0XGB)
    STXS_Category.push_back(cat);

  std::stringstream bdtVarsExpr;
  bdtVarsExpr << "ROOT::RVec<Float_t>{";

  bool first(true);
  for (const std::string &name : features) {
    if (!first)
      bdtVarsExpr << ", ";
    else
      first = false;

    bdtVarsExpr << "static_cast<Float_t>(" << name << ")";
  }
  bdtVarsExpr << "}";

  auto df_classified =
      df.Define("BDT_variables", bdtVarsExpr.str())
          .Define(
              "BDT_Scores",
              [&bdt](const std::vector<Float_t> &vars) {
                return bdt.Compute(vars);
              },
              features)
          .Define("BDT_Category",
                  [](const std::vector<Float_t> &scores) {
                    return std::distance(
                        scores.begin(),
                        std::max_element(scores.begin(), scores.end()));
                  },
                  {"BDT_Scores"})
          .Define("BDT_stage_0",
                  [&STXS_Category](size_t catIdx) {
                    return (catIdx < STXS_Category.size())
                               ? static_cast<int>(STXS_Category[catIdx])
                               : 0;
                  },
                  {"BDT_Category"});

  std::filesystem::path outputFile =
      std::filesystem::path("output/BDTXDG/classification.root");
  df_classified.Snapshot("Events", outputFile.string(),
                         {"EventWeight_lumi18", "EventWeight_lumi9",
                          "EventWeight_lumi138", "EventWeight_lumi250",
                          "EventWeight_lumi300", "EventWeight_lumi350",
                          "trainWeight",

                          "BDT_Category", "BDT_Scores", "BDT_stage_0"});

  exit(EXIT_SUCCESS);
}
