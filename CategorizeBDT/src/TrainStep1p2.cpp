#include "STXSCategorizer/CommonUtils/interface/STXS_Categories.h"
#include <TChain.h>
#include <TCut.h>
#include <TFile.h>
#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>
#include <TString.h>
#include <filesystem>
#include <map>

void loadSTXS1p2Data(TMVA::Factory *factory, const std::string directory,
                     const std::string datasetName,
                     const std::vector<std::string> trainFiles,
                     const bool verbose) {

  static const std::map<STXS_STAGE_1_2_MERGED, std::string> CategoriesToTrain =
      {

          {STXS_STAGE_1_2_MERGED::UNKNOWN, "UNKNOWN"},

          // Gluon fusion
          {STXS_STAGE_1_2_MERGED::GG2H_PTH_GT200, "GG2H_PTH_GT200"},

          {STXS_STAGE_1_2_MERGED::GG2H_0J_PTH_0_10, "GG2H_0J_PTH_0_10"},
          {STXS_STAGE_1_2_MERGED::GG2H_0J_PTH_GT10, "GG2H_0J_PTH_GT10"},

          {STXS_STAGE_1_2_MERGED::GG2H_1J_PTH_0_60, "GG2H_1J_PTH_0_60"},
          {STXS_STAGE_1_2_MERGED::GG2H_1J_PTH_60_120, "GG2H_1J_PTH_60_120"},
          {STXS_STAGE_1_2_MERGED::GG2H_1J_PTH_120_200, "GG2H_1J_PTH_120_200"},

          {STXS_STAGE_1_2_MERGED::GG2H_GE2J_MJJ_0_350_PTH_0_60,
           "GG2H_GE2J_MJJ_0_350_PTH_0_60"},
          {STXS_STAGE_1_2_MERGED::GG2H_GE2J_MJJ_0_350_PTH_60_120,
           "GG2H_GE2J_MJJ_0_350_PTH_60_120"},
          {STXS_STAGE_1_2_MERGED::GG2H_GE2J_MJJ_0_350_PTH_120_200,
           "GG2H_GE2J_MJJ_0_350_PTH_120_200"},
          {STXS_STAGE_1_2_MERGED::GG2H_GE2J_MJJ_GT350, "GG2H_GE2J_MJJ_GT350"},

          // VBF
          {STXS_STAGE_1_2_MERGED::QQ2HQQ_rest, "QQ2HQQ_rest"},

          {STXS_STAGE_1_2_MERGED::QQ2HQQ_GE2J_MJJ_60_120,
           "QQ2HQQ_GE2J_MJJ_60_120"},
          {STXS_STAGE_1_2_MERGED::QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200,
           "QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200"},
          {STXS_STAGE_1_2_MERGED::QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25,
           "QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25"},
          {STXS_STAGE_1_2_MERGED::QQ2HQQ_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25,
           "QQ2HQQ_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25"},

          {STXS_STAGE_1_2_MERGED::QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25,
           "QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25"},
          // qq -> WH
          {STXS_STAGE_1_2_MERGED::VH_lep_PTV_0_150, "VH_lep_PTV_0_150"},

          {STXS_STAGE_1_2_MERGED::VH_lep_PTV_GT150, "VH_lep_PTV_GT150"},

          // ttH
          {STXS_STAGE_1_2_MERGED::TTH, "TTH"},
      };

  std::map<std::string, TCut> categories1p2;

  for (auto [cat, name] : CategoriesToTrain) {
    TCut currentCut = Form("HTXS_stage1_2_cat_pTjet30GeV_merged == %d",
                           static_cast<int>(cat));
    categories1p2[name] = TCut(currentCut);
  }

  std::map<std::string, char> variablesSTXS1p2 = {
      // Discriminants
      {"DVBF2j_ME", 'F'},
      {"DVBF1j_ME", 'F'},
      {"DWHh_ME", 'F'},
      {"DZHh_ME", 'F'},
      {"ZZCand_KD_bestCand", 'F'},

      // ZZ candidate features
      {"ZZCand_pt_bestCand", 'F'},
      {"ZZCand_eta_bestCand", 'F'},
      {"ZZCand_phi_bestCand", 'F'},
      {"ZZCand_mass_bestCand", 'F'},
      {"ZZCand_costheta1_bestCand", 'F'},
      {"ZZCand_costheta2_bestCand", 'F'},
      {"ZZCand_costhetastar_bestCand", 'F'},
      {"ZZCand_Phi1_bestCand", 'F'},
      {"ZZCand_nExtraLep_bestCand", 'I'},

      // Jet features
      {"JetLeading_pt", 'F'},
      {"JetLeading_eta", 'F'},
      {"JetLeading_mass", 'F'},
      {"JetLeading_phi", 'F'},
      {"JetLeading_btag", 'F'},
      {"JetSubleading_pt", 'F'},
      {"JetSubleading_eta", 'F'},
      {"JetSubleading_mass", 'F'},
      {"JetSubleading_phi", 'F'},
      {"JetSubleading_btag", 'F'},

      {"nCleanedJetsPt30", 'I'},
      {"nBtagged_filtered", 'I'},

      // Dijet features
      {"deltaEta_jj", 'F'},
      {"deltaPhi_jj", 'F'},
      {"m_jj", 'F'},

      // Lepton features (primary 4 leptons)
      {"LepPt_0", 'F'},
      {"LepPt_1", 'F'},
      {"LepPt_2", 'F'},
      {"LepPt_3", 'F'},
      {"LepEta_0", 'F'},
      {"LepEta_1", 'F'},
      {"LepEta_2", 'F'},
      {"LepEta_3", 'F'},
      {"LepPhi_0", 'F'},
      {"LepPhi_1", 'F'},
      {"LepPhi_2", 'F'},
      {"LepPhi_3", 'F'},
      {"LepPdgId_0", 'I'},
      {"LepPdgId_1", 'I'},
      {"LepPdgId_2", 'I'},
      {"LepPdgId_3", 'I'},

      // Extra lepton features
      {"LepPt_4", 'F'},
      {"LepPt_5", 'F'},
      {"LepEta_4", 'F'},
      {"LepEta_5", 'F'},
      {"LepPhi_4", 'F'},
      {"LepPhi_5", 'F'},
      {"LepPdgId_4", 'I'},
      {"LepPdgId_5", 'I'},

      // MET
      {"PFMET_pt", 'F'},
  };

  TMVA::DataLoader *loader = new TMVA::DataLoader(datasetName);

  for (auto [varName, dtype] : variablesSTXS1p2) {
    loader->AddVariable(varName, dtype);
  }

  // Get the trees
  std::vector<TTree *> treeList;
  for (std::string filePathStr : trainFiles) {
    std::filesystem::path filePath = std::filesystem::path(filePathStr);

    TFile *inputfile(nullptr);

    if (verbose)
      std::cout << "Reading file " << filePath.string() << std::endl;

    inputfile = TFile::Open(filePath.c_str(), "READ");

    if (!inputfile) {
      std::cerr << "Could not open file " << filePath.string() << std::endl;
      exit(EXIT_FAILURE);
    }

    TTree *tree = (TTree *)inputfile->Get("Events");

    if (!tree || tree->IsZombie()) {
      std::cerr << "Could not get Events tree from file " << filePath.string()
                << std::endl;
      exit(EXIT_FAILURE);
    }

    // Add the data to the loader
    for (auto [name, cut] : categories1p2) {
      loader->AddTree(tree, name, 1.0, cut);
    }

    treeList.push_back(tree); // So that it doesn't get garbage collected
  }

  std::cout << "Finished adding all the trees to the channels to the loader"
            << std::endl;

  loader->SetWeightExpression("trainWeight");
  TCut removeNaN;
  for (auto [name, dtype] : variablesSTXS1p2) {
    removeNaN += Form("!TMath::IsNaN(%s)", name.c_str());
  }

  loader->PrepareTrainingAndTestTree(
      removeNaN, "SplitMode=Random:NormMode=EqualNumEvents");

  int nTrees = 600;
  int maxdepth = 3;
  int minNodeSize = 5;
  int nCuts = 30;
  float Shrinkage = 0.1;
  float BaggedSampleFraction = 0.5;

  std::stringstream bdtString;
  bdtString << ":!H" << ":!V"
            << ":NTrees=" << nTrees << ":MaxDepth=" << maxdepth
            << ":MinNodeSize=" << minNodeSize << "%"
            << ":BoostType=Grad"
            << ":nCuts=" << nCuts << ":UseBaggedBoost=True"
            << ":Shrinkage=" << Shrinkage
            << ":BaggedSampleFraction=" << BaggedSampleFraction
            << ":VarTransform=D,G,N";

  factory->BookMethod(loader, TMVA::Types::kBDT, "BDT", bdtString.str());
}
