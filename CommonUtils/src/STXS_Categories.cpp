#include "STXSCategorizer/CommonUtils/interface/STXS_Categories.h"
#include <cstdlib>

int STXSStage12MergedCat(int cat) {
  if (cat == 102 || cat == 103 || cat == 104)
    return 101;
  if (cat == 114 || cat == 115 || cat == 116)
    return 113;
  if (cat == 201 || cat == 203 || cat == 205)
    return 202;
  if (cat == 210)
    return 208;
  if (cat == 302 || cat == 401 || cat == 402 || cat == 501 || cat == 502)
    return 301;
  if (cat == 304 || cat == 305 || cat == 403 || cat == 404 || cat == 405 ||
      cat == 503 || cat == 504 || cat == 505)
    return 303;
  if (cat == 602 || cat == 603 || cat == 604 || cat == 605)
    return 601;
  return cat;
}

std::string STXSStage12Label(int cat, const std::string &mode) {
  const int merged_cat = STXSStage12MergedCat(cat);
  std::string label;
  switch (merged_cat) {
  case 0:
    label = "UNKNOWN";
    break;
  case 100:
    label = "GG2H_FWDH";
    break;
  case 101:
    label = "GG2H_PTH_GT200";
    break;
  case 105:
    label = "GG2H_0J_PTH_0_10";
    break;
  case 106:
    label = "GG2H_0J_PTH_GT10";
    break;
  case 107:
    label = "GG2H_1J_PTH_0_60";
    break;
  case 108:
    label = "GG2H_1J_PTH_60_120";
    break;
  case 109:
    label = "GG2H_1J_PTH_120_200";
    break;
  case 110:
    label = "GG2H_GE2J_MJJ_0_350_PTH_0_60";
    break;
  case 111:
    label = "GG2H_GE2J_MJJ_0_350_PTH_60_120";
    break;
  case 112:
    label = "GG2H_GE2J_MJJ_0_350_PTH_120_200";
    break;
  case 113:
    label = "GG2H_GE2J_MJJ_GT350";
    break;
  case 200:
    label = "QQ2HQQ_FWDH";
    break;
  case 202:
    label = "QQ2HQQ_rest";
    break;
  case 204:
    label = "QQ2HQQ_GE2J_MJJ_60_120";
    break;
  case 206:
    label = "QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200";
    break;
  case 207:
    label = "QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25";
    break;
  case 208:
    label = "QQ2HQQ_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25";
    break;
  case 209:
    label = "QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25";
    break;
  case 300:
    label = "QQ2HLNU_FWDH";
    break;
  case 301:
    label = "VH_lep_PTV_0_150";
    break;
  case 303:
    label = "VH_lep_PTV_GT150";
    break;
  case 400:
    label = "QQ2HLL_FWDH";
    break;
  case 500:
    label = "GG2HLL_FWDH";
    break;
  case 600:
    label = "TTH_FWDH";
    break;
  case 601:
    label = "TTH";
    break;
  default:
    label = "UNKNOWN";
    break;
  }

  if (label.rfind("QQ2HQQ", 0) == 0) {
    std::string replacement;
    if (mode == "VBFH125") {
      replacement = "VBF";
    } else if (mode == "WplusH125") {
      replacement = "WplushadH";
    } else if (mode == "WminusH125") {
      replacement = "WminushadH";
    } else if (mode == "ZH125") {
      replacement = "ZhadH";
    }

    if (!replacement.empty()) {
      return replacement + label.substr(6);
    }
  }

  if (label.rfind("VH", 0) == 0) {
    std::string replacement;
    if (mode == "WplusH125") {
      replacement = "WplusH";
    } else if (mode == "WminusH125") {
      replacement = "WminusH";
    } else if (mode == "ZH125") {
      replacement = "ZH";
    }

    if (!replacement.empty()) {
      return replacement + label.substr(2);
    }
  }

  return label;
}

STXS_STAGE_1_2_PARTIAL_MERGED STXSStage12PartialMergedCat(STXS_STAGE_1_2 cat) {
  switch (cat) {
  case STXS_STAGE_1_2::QQ2HQQ_0J:
  case STXS_STAGE_1_2::QQ2HQQ_1J:
  case STXS_STAGE_1_2::QQ2HQQ_GE2J_MJJ_0_60:
  case STXS_STAGE_1_2::QQ2HQQ_GE2J_MJJ_120_350:
    return STXS_STAGE_1_2_PARTIAL_MERGED::QQ2HQQ_rest;
  case STXS_STAGE_1_2::QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_GT25:
  case STXS_STAGE_1_2::QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_GT25:
    return STXS_STAGE_1_2_PARTIAL_MERGED::
        QQ2HQQ_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25;
  default:
    return static_cast<STXS_STAGE_1_2_PARTIAL_MERGED>(static_cast<int>(cat));
  }
}

STXS_STAGE_1_2_MERGED STXSStage12MergedCat(STXS_STAGE_1_2_PARTIAL_MERGED cat) {
  switch (cat) {
  case STXS_STAGE_1_2_PARTIAL_MERGED::TTH_PTH_0_60:
  case STXS_STAGE_1_2_PARTIAL_MERGED::TTH_PTH_120_200:
  case STXS_STAGE_1_2_PARTIAL_MERGED::TTH_PTH_200_300:
  case STXS_STAGE_1_2_PARTIAL_MERGED::TTH_PTH_GT300:
    return STXS_STAGE_1_2_MERGED::TTH;
  default:
    return static_cast<STXS_STAGE_1_2_MERGED>(static_cast<int>(cat));
  }
}

STXS_STAGE_1_2_MERGED STXSStage12MergedCat(STXS_STAGE_1_2 cat) {
  return STXSStage12MergedCat(STXSStage12PartialMergedCat(cat));
}
