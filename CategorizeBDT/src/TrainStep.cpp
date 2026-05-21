#include "STXSCategorizer/CategorizeBDT/src/config/TrainConfig.h"
#include <TChain.h>
#include <TCut.h>
#include <TFile.h>
#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>
#include <TString.h>
#include <TSystem.h>
#include <filesystem>
#include <iostream>

template <typename STXS_STAGE>
void loadSTXSData(TMVA::Factory *factory, const std::string directory,
                  const std::string datasetName,
                  const std::vector<std::string> trainFiles,
                  const BDTConfig<STXS_STAGE> &TrainConfig,
                  const bool verbose) {

  std::vector<std::pair<std::string, TCut>> cutPerCategory;

  for (auto [cat, name] : TrainConfig.categoryNameMap) {
    cutPerCategory.emplace_back(
        name, TCut(Form("%s == %d", TrainConfig.discriminantColumn.c_str(),
                        static_cast<int>(cat))));
  }

  gSystem->ChangeDirectory(directory.c_str());
  TMVA::DataLoader *loader = new TMVA::DataLoader(datasetName);

  for (auto [varName, dtype] : TrainConfig.variables) {
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
    for (auto [name, cut] : cutPerCategory) {
      loader->AddTree(tree, name, 1.0, cut);
      if (verbose)
        std::cout << "Loaded " << tree->GetEntries(cut)
                  << " events to category " << name << " with cut " << cut
                  << " on tree of file " << filePathStr << std::endl;
    }

    treeList.push_back(tree); // So that it doesn't get garbage collected
  }

  std::cout << "Finished adding all the trees to the channels to the loader"
            << std::endl;

  loader->SetWeightExpression("trainWeight");
  TCut removeNaN;
  for (auto [name, dtype] : TrainConfig.variables) {
    removeNaN += Form("!TMath::IsNaN(%s)", name.c_str());
  }

  loader->PrepareTrainingAndTestTree(
      removeNaN, "SplitMode=Random:NormMode=EqualNumEvents");

  factory->BookMethod(loader, TMVA::Types::kBDT, "BDT",
                      TrainConfig.hyperparams.BDTString());
}
