#include "STXSCategorizer/CategorizeBDT/src/config/Categories.h"
#include "STXSCategorizer/CategorizeBDT/src/config/Parameters.h"
#include "STXSCategorizer/CategorizeBDT/src/config/Variables.h"
#include <TChain.h>
#include <TCut.h>
#include <TFile.h>
#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>
#include <TString.h>
#include <filesystem>
#include <iostream>

void loadSTXS1p2Data(TMVA::Factory *factory, const std::string directory,
                     const std::string datasetName,
                     const std::vector<std::string> trainFiles,
                     const bool verbose) {

  std::vector<std::pair<std::string, TCut>> categories1p2;

  for (auto [cat, name] :
       STXS1p2TrainConfig::CategoriesToTrain /* From config/Categories.h */) {
    categories1p2.emplace_back(
        name, TCut(Form("HTXS_stage1_2_cat_pTjet30GeV_merged == %d",
                        static_cast<int>(cat))));
  }

  TMVA::DataLoader *loader = new TMVA::DataLoader(datasetName);

  for (auto [varName, dtype] : variablesSTXS1p2 /* From config/Variables.h */) {
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
      if (verbose)
        std::cout << "Loaded category " << name << " with cut " << cut
                  << " on tree of file " << filePathStr << std::endl;
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

  std::stringstream bdtString;
  bdtString << ":!H" << ":!V"
            << ":NTrees=" << STXS1p2Params::nTrees
            << ":MaxDepth=" << STXS1p2Params::maxdepth
            << ":MinNodeSize=" << STXS1p2Params::minNodeSize << "%"
            << ":BoostType=Grad"
            << ":nCuts=" << STXS1p2Params::nCuts << ":UseBaggedBoost=True"
            << ":Shrinkage=" << STXS1p2Params::Shrinkage
            << ":BaggedSampleFraction=" << STXS1p2Params::BaggedSampleFraction
            << ":VarTransform=D,G,N";

  if (verbose)
    std::cout << "Training the BDT with the following configuration\n\t"
              << bdtString.str() << std::endl;

  factory->BookMethod(loader, TMVA::Types::kBDT, "BDT", bdtString.str());
}
