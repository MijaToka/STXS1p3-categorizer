#include "STXSCategorizer/CommonUtils/interface/STXS_Categories.h"
#include <map>
#include <sstream>
#include <string>

struct HyperParams {
  int nTrees;
  int maxdepth;
  int minNodeSize;
  int nCuts;
  float Shrinkage;
  float BaggedSampleFraction;

  std::string BDTString() {
    std::stringstream ss;
    ss << ":!H" << ":!V"
       << ":NTrees=" << nTrees << ":MaxDepth=" << maxdepth
       << ":MinNodeSize=" << minNodeSize << "%"
       << ":BoostType=Grad"
       << ":nCuts=" << nCuts << ":UseBaggedBoost=True"
       << ":Shrinkage=" << Shrinkage
       << ":BaggedSampleFraction=" << BaggedSampleFraction
       << ":VarTransform=D,G,N";
    return ss.str();
  };
};

struct BDTTrainConfig {
  BDTTrainConfig();
  HyperParams hyperparams;
  std::map<std::string, char> variables;
  std::map<STXS_STAGE_1_2_MERGED, std::string> categories;
};
