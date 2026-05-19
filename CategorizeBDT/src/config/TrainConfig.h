#ifndef TRAINCONFIG_H
#define TRAINCONFIG_H

#include "STXSCategorizer/CategorizeBDT/src/config/Categories.h"
#include "STXSCategorizer/CategorizeBDT/src/config/Variables.h"
#include "STXSCategorizer/CommonUtils/interface/STXS_Categories.h"
#include <TMath.h>
#include <map>
#include <sstream>
#include <string>

struct HyperParams {
  int nTrees = 800;
  int maxdepth = 5;
  int nCuts = 50;
  float minNodeSize = 2.5;
  float Shrinkage = 0.05;
  float BaggedSampleFraction = 0.6;

  std::string BDTString() const {
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

template <typename STXS_STAGE> struct BDTConfig {
  HyperParams hyperparams;
  std::map<std::string, char> variables;
  std::string discriminantColumn;
  std::string classificationColumn;
  std::map<STXS_STAGE, std::string> categoryNameMap;
  std::vector<STXS_STAGE> categories;
  std::vector<std::string> categoryNames;

  STXS_STAGE getNthCategory(size_t catIdx) {
    if (categories.empty()) {
      for (auto &[cat, name] : categoryNameMap)
        categories.push_back(cat);
    }
    return (catIdx < categories.size()) ? categories[catIdx]
                                        : static_cast<STXS_STAGE>(0);
  };

  std::string getNthName(size_t catIdx) {
    if (categoryNames.empty()) {
      for (auto &[cat, name] : categoryNameMap)
        categoryNames.push_back(name);
    }
    return (catIdx < categories.size()) ? categoryNames[catIdx] : "UNKNOWN";
  };
};

inline BDTConfig<STXS_STAGE_1_2_MERGED> TrainConfig1p2{
    .hyperparams = HyperParams{},
    .variables = variablesSTXS1p2 /* from Variables.h */,
    .discriminantColumn = "HTXS_stage1_2_cat_pTjet30GeV_merged",
    .classificationColumn = "BDT_stage1_2_cat_pTjet30GeV_merged",
    .categoryNameMap =
        STXSTrainCategories::CategoriesToTrain /* from Categories.h */
};

inline BDTConfig<STXS_STAGE_0> TrainConfig0{
    .hyperparams = HyperParams{},
    .variables = variablesSTXS1p2,
    .discriminantColumn = "HTXS_stage_0",
    .classificationColumn = "BDT_stage_0",
    .categoryNameMap = STXSTrainCategories::CategoriesToTrainStage0};

#endif
