#ifndef TRAINCONFIG_H
#define TRAINCONFIG_H

#include "STXSCategorizer/CategorizeBDT/src/config/Categories.h"
#include "STXSCategorizer/CategorizeBDT/src/config/Variables.h"
#include "STXSCategorizer/CommonUtils/interface/STXS_Categories.h"
#include <RtypesCore.h>
#include <TMath.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

struct HyperParams {
  int nTrees = 800;
  int maxdepth = 5;
  int nCuts = 50;
  float minNodeSize = 2.5;
  float Shrinkage = 0.05;
  float BaggedSampleFraction = 0.6;
  std::string VarTransform = "N,G,D";

  std::string BDTString() const {
    std::stringstream ss;
    ss << ":!H" << ":!V"
       << ":NTrees=" << nTrees << ":MaxDepth=" << maxdepth
       << ":MinNodeSize=" << minNodeSize << "%"
       << ":BoostType=Grad"
       << ":nCuts=" << nCuts << ":UseBaggedBoost=True"
       << ":Shrinkage=" << Shrinkage
       << ":BaggedSampleFraction=" << BaggedSampleFraction
       << ":VarTransform=" << VarTransform;
    return ss.str();
  };
};

struct BDTConfigBase {
  virtual ~BDTConfigBase() = default;
  HyperParams hyperparams;
  std::map<std::string, char> variables;
  std::string discriminantColumn;
  std::string classificationColumn;
  std::vector<std::string> categoryNames;
  virtual std::string getNthName(size_t catIdx) = 0;
  virtual int getNthCategory(size_t catIdx) = 0;
  virtual Int_t getBDTCategory(Int_t HTXS_cat) = 0;
  virtual void initializeCategoryVectors() = 0;
};

template <typename STXS_STAGE> struct BDTConfig : public BDTConfigBase {

  std::map<STXS_STAGE, std::string> categoryNameMap;
  std::vector<STXS_STAGE> categories;

  BDTConfig(HyperParams hp, std::map<std::string, char> vars,
            std::string discCol, std::string classCol,
            std::map<STXS_STAGE, std::string> catNameMap) {
    hyperparams = hp;
    variables = std::move(vars);
    discriminantColumn = std::move(discCol);
    classificationColumn = std::move(classCol);
    categoryNameMap = std::move(catNameMap);
    initializeCategoryVectors();
  }

  void initializeCategoryVectors() {
    for (auto &[cat, name] : categoryNameMap) {
      categories.push_back(cat);
      categoryNames.push_back(name);
    }
  }
  int getNthCategory(size_t catIdx) {
    return (catIdx < categories.size()) ? static_cast<int>(categories[catIdx])
                                        : 0;
  };

  std::string getNthName(size_t catIdx) {
    return (catIdx < categories.size()) ? categoryNames[catIdx] : "UNKNOWN";
  };

  Int_t getBDTCategory(Int_t HTXS_cat) {
    auto it = find(categories.begin(), categories.end(),
                   static_cast<STXS_STAGE>(HTXS_cat));
    if (it == categories.end())
      return -1;
    else
      return std::distance(categories.begin(), it);
  }
};

inline BDTConfig<STXS_STAGE_1_2_MERGED> TrainConfig1p2(
    /* .hyperparams = */ HyperParams{},
    /* .variables = */
    STXSTrainVariables::variablesSTXS1p2,
    /* .discriminantColumn = */ "HTXS_stage1_2_cat_pTjet30GeV_merged",
    /* .classificationColumn = */ "BDT_stage1_2_cat_pTjet30GeV_merged",
    /* .categoryNameMap = */
    STXSTrainCategories::CategoriesToTrain);

inline BDTConfig<STXS_STAGE_0> TrainConfig0{
    /* .hyperparams = */ HyperParams{
        .nTrees = 600, .maxdepth = 10, .Shrinkage = 0.1},
    /* .variables = */ STXSTrainVariables::variablesSTXS1p2,
    /* .discriminantColumn = */ "HTXS_stage_0",
    /* .classificationColumn = */ "BDT_stage_0",
    /* .categoryNameMap = */ STXSTrainCategories::CategoriesToTrainStage0};

inline BDTConfig<STXS_STAGE_0> TrainConfig0XGB{
    /* .hyperparams = */ HyperParams{
        .nTrees = 100, .maxdepth = 4, .Shrinkage = 0.01},
    /* .variables = */ STXSTrainVariables::variablesSTXS0XGB,
    /* .discriminantColumn = */ "HTXS_stage_0",
    /* .classificationColumn = */ "BDT_stage_0",
    /* .categoryNameMap = */ STXSTrainCategories::CategoriesToTrainStage0XGB};

#endif
