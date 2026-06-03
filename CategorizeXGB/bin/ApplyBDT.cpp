#include "STXSCategorizer/CategorizeBDT/src/config/Categories.h"
#include "STXSCategorizer/CommonUtils/interface/STXS_Categories.h"
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <TDictionary.h>
#include <TMVA/RBDT.hxx>
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  std::vector<std::string> columns_to_save = {
      // ZZ candidate features
      "ZZCand_pt", "ZZCand_pt_bestCand", "ZZCand_eta", "ZZCand_eta_bestCand",
      "ZZCand_phi", "ZZCand_phi_bestCand", "ZZCand_mass",
      "ZZCand_mass_bestCand", "ZZCand_costheta1", "ZZCand_costheta1_bestCand",
      "ZZCand_costheta2", "ZZCand_costheta2_bestCand", "ZZCand_costhetastar",
      "ZZCand_costhetastar_bestCand", "ZZCand_Phi1", "ZZCand_Phi1_bestCand",
      "ZZCand_nExtraLep", "ZZCand_nExtraLep_bestCand", "ZZjj_pt", "bestCandIdx",

      // MET
      "PFMET_pt",

      // Jet features
      "JetLeading_pt", "JetLeading_eta", "JetLeading_mass", "JetLeading_phi",
      "JetSubleading_pt", "JetSubleading_eta", "JetSubleading_mass",
      "JetSubleading_phi", "nCleanedJetsPt30", "nBtagged_filtered",
      "JetLeading_btag", "JetSubleading_btag",

      // Dijet features
      "deltaEta_jj", "deltaPhi_jj", "m_jj",

      // Lepton features (primary 4 leptons)
      "LepPt_0", "LepPt_1", "LepPt_2", "LepPt_3", "LepEta_0", "LepEta_1",
      "LepEta_2", "LepEta_3", "LepPhi_0", "LepPhi_1", "LepPhi_2", "LepPhi_3",
      "LepPdgId_0", "LepPdgId_1", "LepPdgId_2", "LepPdgId_3",

      // Extra lepton features
      "LepPt_4", "LepPt_5", "LepEta_4", "LepEta_5", "LepPhi_4", "LepPhi_5",
      "LepPdgId_4", "LepPdgId_5",

      // Discriminants
      "DVBF2j_ME", "DVBF1j_ME", "DWHh_ME", "DZHh_ME", "DVBF2j_ME_noC",
      "DVBF1j_ME_noC", "DWHh_ME_noC", "DZHh_ME_noC", "ZZCand_KD",
      "ZZCand_KD_bestCand",

      // Event weights
      "EventWeight_lumi18", "EventWeight_lumi9", "EventWeight_lumi138",
      "EventWeight_lumi250", "EventWeight_lumi300", "EventWeight_lumi350",
      "production_mode", "overallEventWeight", "genWeight", "puWeight",
      "trainWeight", "genEventSumw",

      // MC STXS categories
      "HTXS_stage_0", "HTXS_stage1_2_cat_pTjet30GeV",
      "HTXS_stage1_2_cat_pTjet30GeV_merged",
      "HTXS_stage1_2_cat_pTjet30GeV_label"};
  std::vector<std::string> signals = {"gg",    "VBF", "Wminus",
                                      "Wplus", "Z",   "tt"};
  std::vector<std::string> files;
  for (std::string signal : signals) {
    std::stringstream ss;
    ss << "output/train/" << signal << "H125/snapshot.root";
    std::cout << "Adding file: " << ss.str() << std::endl;
    files.push_back(ss.str());
  }

  if (argc > 1) {
    for (std::string mode :
         {"ZZTo4l", "WWZ", "ZZZ", "ggTo4mu_Contin_MCFM701",
          "ggTo2e2mu_Contin_MCFM701", "ggTo2mu2tau_Contin_MCFM701", "TTZZ",
          "ggTo2e2tau_Contin_MCFM701", "TTWW", "ggTo4tau_Contin_MCFM701",
          "ggTo4e_Contin_MCFM701"}) {
      std::stringstream ss;
      ss << "output/PROD_samplesNano_2022_MC_8d4c03f7/" << mode
         << "/snapshot.root";
      std::cout << "Adding file: " << ss.str() << std::endl;
      files.push_back(ss.str());
    }
  }

  TMVA::Experimental::RBDT bdt("multiclassXGB", "multiclassXGB1p2_model.root");
  ROOT::RDataFrame df("Events", files);
  const std::vector<std::string> features = {
      "DVBF1j_ME",
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
      //"nCleanedJetsPt30"
  };
  std::vector<STXS_STAGE_1_2_MERGED> STXS_Category;
  for (auto &[cat, name] : STXSTrainCategories::CategoriesToTrainStage1p2XGB)
    STXS_Category.push_back(cat);

  std::stringstream bdtVarsExpr;
  bdtVarsExpr << "ROOT::RVec<Float_t>{";

  std::string separator("");
  for (const std::string &name : features) {
    bdtVarsExpr << separator << "static_cast<Float_t>(" << name << ")";
    separator = ", ";
  }
  bdtVarsExpr << "}";

  auto df_classified =
      df.Define("BDT_variables", bdtVarsExpr.str())
          .Define("BDT_Scores",
                  [&bdt](const ROOT::RVec<Float_t> &vars) {
                    return bdt.Compute(vars);
                  },
                  {"BDT_variables"})
          .Define("BDT_Category",
                  [](const ROOT::RVec<Float_t> &scores) -> size_t {
                    return std::distance(
                        scores.begin(),
                        std::max_element(scores.begin(), scores.end()));
                  },
                  {"BDT_Scores"})
          .Define("BDT_stage_0",
                  [&STXS_Category](size_t catIdx) -> int {
                    return (catIdx < STXS_Category.size())
                               ? static_cast<int>(STXS_Category[catIdx])
                               : 0;
                  },
                  {"BDT_Category"});

  std::vector<std::string> BDT_columns = {"BDT_Category", "BDT_Scores",
                                          "BDT_stage_0"};

  columns_to_save.insert(columns_to_save.end(), BDT_columns.begin(),
                         BDT_columns.end());

  std::filesystem::path outputFile =
      std::filesystem::path("output/BDTXGB/classification1p2.root");
  df_classified.Snapshot("Events", outputFile.string(), columns_to_save);

  exit(EXIT_SUCCESS);
}
