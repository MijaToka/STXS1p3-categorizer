
#include <TCut.h>
#include <TMVA/Factory.h>
void loadSTXS1p2Data(TMVA::Factory, const std::string directory,
                     const std::string datasetName, const bool verbose) {

  std::map<std::string, TCut> categories1p2 = {
      {"GG2H_0J_PTH_0_10",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"GG2H_0J_PTH_0_10\"")},
      {"GG2H_0J_PTH_GT10",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"GG2H_0J_PTH_GT10\"")},
      {"GG2H_1J_PTH_0_60",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"GG2H_1J_PTH_0_60\"")},
      {"GG2H_1J_PTH_120_200",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"GG2H_1J_PTH_120_200\"")},
      {"GG2H_1J_PTH_60_120",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"GG2H_1J_PTH_60_120\"")},
      {"GG2H_GE2J_MJJ_0_350_PTH_0_60",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == "
            "\"GG2H_GE2J_MJJ_0_350_PTH_0_60\"")},
      {"GG2H_GE2J_MJJ_0_350_PTH_120_200",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == "
            "\"GG2H_GE2J_MJJ_0_350_PTH_120_200\"")},
      {"GG2H_GE2J_MJJ_0_350_PTH_60_120",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == "
            "\"GG2H_GE2J_MJJ_0_350_PTH_60_120\"")},
      {"GG2H_GE2J_MJJ_GT350",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"GG2H_GE2J_MJJ_GT350\"")},
      {"GG2H_PTH_GT200",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"GG2H_PTH_GT200\"")},
      {"QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == "
            "\"QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25\"")},
      {"QQ2HQQ_GE2J_MJJ_60_120",
       TCut(
           "HTXS_stage1_2_cat_pTjet30GeV_label == \"QQ2HQQ_GE2J_MJJ_60_120\"")},
      {"QQ2HQQ_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == "
            "\"QQ2HQQ_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25\"")},
      {"QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == "
            "\"QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200\"")},
      {"QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == "
            "\"QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25\"")},
      {"QQ2HQQ_rest",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"QQ2HQQ_rest\"")},
      {"TTH", TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"TTH\"")},
      {"TTH_FWDH", TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"TTH_FWDH\"")},
      {"UNKNOWN", TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"UNKNOWN\"")},
      {"VH_lep_PTV_0_150",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"VH_lep_PTV_0_150\"")},
      {"VH_lep_PTV_GT150",
       TCut("HTXS_stage1_2_cat_pTjet30GeV_label == \"VH_lep_PTV_GT150\"")},
  };

  std::map<std::string, char> variablesSTXS1p2 = {
      // Discriminants
      {"DVBF2j_ME", 'F'},
      {"DVBF1j_ME", 'F'},
      {"DWHh_ME", 'F'},
      {"DZHh_ME", 'F'},
      {"ZZCand_KD_bestCand", 'F'},

      // ZZ candidate features
      "ZZCand_pt_bestCand",
      "ZZCand_eta_bestCand",
      "ZZCand_phi_bestCand",
      "ZZCand_mass_bestCand",
      "ZZCand_costheta1_bestCand",
      "ZZCand_costheta2_bestCand",
      "ZZCand_costhetastar_bestCand",
      "ZZCand_Phi1_bestCand",
      "ZZCand_nExtraLep_bestCand",
      "ZZjj_pt",

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

      // MET
      "PFMET_pt",
  };
}
