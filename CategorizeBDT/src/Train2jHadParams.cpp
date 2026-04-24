#include <TFile.h>
#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>
#include <filesystem>
#include <map>
#include <string>

void load2jData(TMVA::Factory *factory, const std::string directory,
                const std::string dataseName, const bool verbose) {

  TCut hadCut("ZZCand_nExtraLep_best == 0");
  TCut cut2jet("nCleanedJetsPt30 >= 2");

  std::map<std::string, std::pair<std::string, TCut>> categoriesPerSignal2jet =
      {{"ttH125", {"ttH_hadronic", cut2jet + hadCut}},
       {"WminusH125", {"VH_hadronic", cut2jet + hadCut}},
       {"WplusH125", {"VH_hadronic", cut2jet + hadCut}},
       {"ZH125", {"VH_hadronic", cut2jet + hadCut}},
       {"VBFH125", {"VBF", cut2jet + hadCut}},
       {"ggH125", {"ggH", cut2jet + hadCut}}};

  TMVA::DataLoader *loader = new TMVA::DataLoader(dataseName);

  // Initialize the variables that the BDT will categorize on
  std::vector<std::pair<std::string, char>> variables2j = {
      {"nBtagged_filtered", 'I'},
      {"nCleanedJetsPt30", 'I'},
      {"DVBF2j_ME", 'F'},
      {"DWHh_ME", 'F'},
      {"DZHh_ME", 'F'}};

  for (auto [varName, dtype] : variables2j) {
    loader->AddVariable(varName, dtype);
  }

  // Retrieve the trees
  std::map<std::string, TTree *> treeMap;

  for (auto [mode, _] : categoriesPerSignal2jet) {
    std::filesystem::path filePath =
        std::filesystem::path(directory) / mode / "snapshot.root";
    TFile *inputfile(nullptr);

    if (verbose)
      std::cout << "Reading the file " << filePath.string() << std::endl;

    inputfile = TFile::Open(filePath.c_str(), "READ");

    if (!inputfile) {
      std::cerr << "Could not open file " << filePath.string() << std::endl;
      exit(EXIT_FAILURE);
    }

    treeMap[mode] = (TTree *)inputfile->Get("Events");
    if (!treeMap[mode] || treeMap[mode]->IsZombie()) {
      std::cerr << "Could not get Events tree from file " << filePath.string()
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  TCut removeNaN("isfinite(DVBF2j_ME) && isfinite(DVBF1j_ME) && "
                 "isfinite(DWHh_ME) && isfinite(DZHh_ME)");

  for (auto [mode, tree] : treeMap) {
    auto [category, cut] = categoriesPerSignal2jet[mode];
    loader->AddTree(tree, category, 1.0, cut + removeNaN);
  }

  loader->SetWeightExpression("trainWeight");
  loader->PrepareTrainingAndTestTree(
      "", "", "SplitMode=Random:NormMode=EqualNumEvents");

  int nTrees = 300;
  int maxdepth = 3;
  int minNodeSize = 20;
  int nCuts = 30;

  std::stringstream bdtString;
  bdtString << ":!H" << ":!V"
            << ":NTrees=" << nTrees << ":MaxDepth=" << maxdepth
            << ":MinNodeSize=" << minNodeSize << "%"
            << ":BoostType=Grad"
            << ":nCuts=" << nCuts << ":UseBaggedBoost=True";

  factory->BookMethod(loader, TMVA::Types::kBDT, "BDT", bdtString.str());
}
