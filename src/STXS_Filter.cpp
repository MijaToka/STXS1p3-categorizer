#include "Preprocess.cpp"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterfaceBase.hxx>
#include <ROOT/RDataFrame.hxx>
#include <RtypesCore.h>
#include <cstdlib>
#include <map>
#include <sstream>
#include <string>
#include <vector>

void snapshot(ROOT::RDF::RNode df, std::string output_dir) {
  std::vector<std::string> column_to_save = {
      // ZZ candidate features
      "ZZCand_pt",
      "ZZCand_eta",
      "ZZCand_phi",
      "ZZCand_mass",
      "ZZCand_costheta1",
      "ZZCand_costheta2",
      "ZZCand_costhetastar",
      "ZZCand_Phi1",
      "ZZCand_nExtraLep",
      "ZZjj_pt",
      // MET
      "PFMET_pt",
      // Jet features
      "JetLeading_pt",
      "JetLeading_eta",
      "JetLeading_mass",
      "JetLeading_phi",
      "JetSubleading_pt",
      "JetSubleading_eta",
      "JetSubleading_mass",
      "JetSubleading_phi",
      "nCleanedJetsPt30",
      "nBtagged_filtered",
      "JetLeading_btag",
      "JetSubleading_btag",
      // Dijet features
      "deltaEta_jj",
      "deltaPhi_jj",
      "m_jj",
      // Lepton features (primary 4 leptons)
      "LepPt_0",
      "LepPt_1",
      "LepPt_2",
      "LepPt_3",
      "LepEta_0",
      "LepEta_1",
      "LepEta_2",
      "LepEta_3",
      "LepPhi_0",
      "LepPhi_1",
      "LepPhi_2",
      "LepPhi_3",
      "LepPdgId_0",
      "LepPdgId_1",
      "LepPdgId_2",
      "LepPdgId_3",
      // Extra lepton features
      "LepPt_4",
      "LepPt_5",
      "LepEta_4",
      "LepEta_5",
      "LepPhi_4",
      "LepPhi_5",
      "LepPdgId_4",
      "LepPdgId_5",
      // Discriminants
      "DVBF2j_ME",
      "DVBF1j_ME",
      "DWHh_ME",
      "DZHh_ME",
      "DVBF2j_ME_noC",
      "DVBF1j_ME_noC",
      "DWHh_ME_noC",
      "DZHh_ME_noC",
      "ZZCand_KD",
      // Event weights and label
      /*
        "EventWeight_lumi18", "EventWeight_lumi9", "EventWeight_lumi138",
        "production_mode", "HTXS_stage_0", "overallEventWeight", "genWeight",
        "puWeight", "trainWeight", "HTXS_stage1_2_cat_pTjet30GeV",
        "HTXS_stage1_2_cat_pTjet30GeV_label", "genEventSumw"
      */
  };

  std::stringstream ss;
  ss << output_dir << "/snapshot.root";
  df.Snapshot("Events", ss.str(), column_to_save);
}

/*std::map<std::string, ROOT::RDataFrame> STXS_step0(ROOT::RDataFrame df) {
  std::map<std::string, ROOT::RDataFrame> output;
  output["VBF_2jet_tagged"] = df.Filter(
      [](Int_t ZZCand_nExtraLep, float DVBF2j_ME, Char_t nCleanedJetsPt30,
         Char_t nBtagged_filtered) {
        return (ZZCand_nExtraLep == 0) && (DVBF2j_ME > 0.5) &&
               ((((nCleanedJetsPt30 == 2) || (nCleanedJetsPt30 == 3)) &&
                 (nBtagged_filtered <= 1)) ||
                ((nCleanedJetsPt30 == 4) && (nBtagged_filtered == 0)));
      },
      {"ZZCand_nExtraLep", "DVBF2j_ME", "nCleanedJetsPt30",
       "nBtagged_filtered"});

  return output;
}*/

void STXS_Filter(std::vector<std::string> files) {
  auto df = setup(files);
  snapshot(df, "data");
}
