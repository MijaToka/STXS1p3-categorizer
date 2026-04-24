#include "STXSCategorizer/CategorizeBDT/src/Train2jHadParams.cpp"
#include "STXSCategorizer/CategorizeBDT/src/parseArgs.cpp"
#include "STXSCategorizer/CommonUtils/interface/Preprocess.h"
#include "STXSCategorizer/CommonUtils/interface/STXS_common.h"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterfaceBase.hxx>
#include <ROOT/RDataFrame.hxx>
#include <TCut.h>
#include <TFile.h>
#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>
#include <TMVA/Tools.h>
#include <TMVA/Types.h>
#include <TROOT.h>
#include <TTree.h>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  ROOT::EnableImplicitMT();

  std::string directory, output, dataSetName;
  bool preprocess, verbose;

  parseArguments(argc, argv, directory, output, preprocess, verbose);

  dataSetName = std::filesystem::path(directory).filename().string();

  std::vector<std::string> signalModes = {"ggH125",    "VBFH125",    "ZH125",
                                          "WplusH125", "WminusH125", "ttH125"};

  std::vector<std::string> bkgModes = {"ZZTo4l",
                                       "WWZ",
                                       "ZZZ",
                                       "ggTo4mu_Contin_MCFM701",
                                       "ggTo2e2mu_Contin_MCFM701",
                                       "ggTo2mu2tau_Contin_MCFM701",
                                       "TTZZ",
                                       "ggTo2e2tau_Contin_MCFM701",
                                       "TTWW",
                                       "ggTo4tau_Contin_MCFM701",
                                       "ggTo4e_Contin_MCFM701"};

  // Define what files are going to be in the training and which ones have to
  // preprocessed
  std::vector<std::string> trainFiles;
  for (const std::string mode : signalModes) {

    std::stringstream output_dir, input_file;

    if (preprocess) {
      std::filesystem::path inPath(directory);

      input_file << directory << "/" << mode << "/ZZ4lAnalysis.root";
      output_dir << output << "/" << dataSetName << "/" << mode;

      if (verbose)
        std::cout << "Preprocessing file: " << input_file.str() << std::endl;

      std::string filestr = input_file.str();

      ROOT::RDF::RNode df = setup(filestr, mode);

      snapshot(df, output_dir.str());

      output_dir << "/snapshot.root";

      if (verbose)
        std::cout << "Writting snapshot to " << output_dir.str() << std::endl;

      trainFiles.push_back(output_dir.str());
    } else {
      input_file << directory << "/" << mode << "/snapshot.root";
      trainFiles.push_back(input_file.str());
    }
  }

  if (preprocess) {
    std::stringstream ss;
    ss << output << "/" << dataSetName;
    directory = ss.str();
  }

  // Now its time to train

  // Initialize the factory and data loader
  std::stringstream outfiless;
  outfiless << output << "/" << dataSetName << "/train_BDT_results.root";
  TFile *outfile = TFile::Open(outfiless.str().c_str(), "RECREATE");

  TMVA::Factory *factory = new TMVA::Factory(
      "TMVAMulticlassSTXS0", outfile,
      ":!V:!Silent:Color:DrawProgressBar:AnalysisType=Multiclass");

  load2jData(factory, directory, dataSetName, verbose);

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();

  outfile->Close();
  if (verbose) {
    std::cout << "==> Wrote root file: " << outfile->GetName() << std::endl;
    std::cout << "==> TMVAMulticlass is done!" << std::endl;
  }

  delete factory;
  return 1;
}
