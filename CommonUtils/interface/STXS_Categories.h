#ifndef STXS_CATEGORIES_H
#define STXS_CATEGORIES_H

#include <map>
#include <string>
#include <vector>
enum class STXS_STAGE_0 {
  UNKNOWN = 0,
  GG2H_FWDH = 10,
  GG2H = 11,
  VBF_FWDH = 20,
  VBF = 21,
  VH2HQQ_FWDH = 22,
  VH2HQQ = 23,
  QQ2HLNU_FWDH = 30,
  QQ2HLNU = 31,
  QQ2HLL_FWDH = 40,
  QQ2HLL = 41,
  GG2HLL_FWDH = 50,
  GG2HLL = 51,
  TTH_FWDH = 60,
  TTH = 61,
  BBH_FWDH = 70,
  BBH = 71,
  TH_FWDH = 80,
  TH = 81
};

enum class STXS_STAGE_1_2 {
  UNKNOWN = 0,
  // Gluon fusion
  GG2H_FWDH = 100,
  GG2H_PTH_200_300 = 101,
  GG2H_PTH_300_450 = 102,
  GG2H_PTH_450_650 = 103,
  GG2H_PTH_GT650 = 104,
  GG2H_0J_PTH_0_10 = 105,
  GG2H_0J_PTH_GT10 = 106,
  GG2H_1J_PTH_0_60 = 107,
  GG2H_1J_PTH_60_120 = 108,
  GG2H_1J_PTH_120_200 = 109,
  GG2H_GE2J_MJJ_0_350_PTH_0_60 = 110,
  GG2H_GE2J_MJJ_0_350_PTH_60_120 = 111,
  GG2H_GE2J_MJJ_0_350_PTH_120_200 = 112,
  GG2H_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25 = 113,
  GG2H_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_GT25 = 114,
  GG2H_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25 = 115,
  GG2H_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_GT25 = 116,
  // VBF
  QQ2HQQ_FWDH = 200,
  QQ2HQQ_0J = 201,
  QQ2HQQ_1J = 202,
  QQ2HQQ_GE2J_MJJ_0_60 = 203,
  QQ2HQQ_GE2J_MJJ_60_120 = 204,
  QQ2HQQ_GE2J_MJJ_120_350 = 205,
  QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200 = 206,
  QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25 = 207,
  QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_GT25 = 208,
  QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25 = 209,
  QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_GT25 = 210,
  // qq -> WH
  QQ2HLNU_FWDH = 300,
  QQ2HLNU_PTV_0_75 = 301,
  QQ2HLNU_PTV_75_150 = 302,
  QQ2HLNU_PTV_150_250_0J = 303,
  QQ2HLNU_PTV_150_250_GE1J = 304,
  QQ2HLNU_PTV_GT250 = 305,
  // qq -> ZH
  QQ2HLL_FWDH = 400,
  QQ2HLL_PTV_0_75 = 401,
  QQ2HLL_PTV_75_150 = 402,
  QQ2HLL_PTV_150_250_0J = 403,
  QQ2HLL_PTV_150_250_GE1J = 404,
  QQ2HLL_PTV_GT250 = 405,
  // gg -> ZH
  GG2HLL_FWDH = 500,
  GG2HLL_PTV_0_75 = 501,
  GG2HLL_PTV_75_150 = 502,
  GG2HLL_PTV_150_250_0J = 503,
  GG2HLL_PTV_150_250_GE1J = 504,
  GG2HLL_PTV_GT250 = 505,
  // ttH
  TTH_FWDH = 600,
  TTH_PTH_0_60 = 601,
  TTH_PTH_60_120 = 602,
  TTH_PTH_120_200 = 603,
  TTH_PTH_200_300 = 604,
  TTH_PTH_GT300 = 605,
  // bbH
  BBH_FWDH = 700,
  BBH = 701,
  // tH
  TH_FWDH = 800,
  TH = 801
};

enum class STXS_STAGE_1_2_MERGED {
  UNKNOWN = 0,
  // Gluon fusion
  GG2H_FWDH = 100,
  GG2H_PTH_GT200 = 101, // 101, 102, 103, 104 merged into one category
  GG2H_0J_PTH_0_10 = 105,
  GG2H_0J_PTH_GT10 = 106,
  GG2H_1J_PTH_0_60 = 107,
  GG2H_1J_PTH_60_120 = 108,
  GG2H_1J_PTH_120_200 = 109,
  GG2H_GE2J_MJJ_0_350_PTH_0_60 = 110,
  GG2H_GE2J_MJJ_0_350_PTH_60_120 = 111,
  GG2H_GE2J_MJJ_0_350_PTH_120_200 = 112,
  GG2H_GE2J_MJJ_GT350 = 113, // 113, 114, 115, 116 merged into one category
  // VBF
  QQ2HQQ_FWDH = 200,
  QQ2HQQ_rest = 202, // 201,202,203,205 merged into one category
  QQ2HQQ_GE2J_MJJ_60_120 = 204,
  QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200 = 206,
  QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25 = 207,
  QQ2HQQ_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25 =
      208, // 208, 210 merged into one category
  QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25 = 209,
  // qq -> WH
  QQ2HLNU_FWDH = 300,
  VH_lep_PTV_0_150 =
      301, // 301, 302, 401, 402, 501, 502 merged into one category
  VH_lep_PTV_GT150 = 303, // 303, 304, 305, 403, 404, 405, 503, 504, 505 merged
                          // into one category
  // qq -> ZH
  QQ2HLL_FWDH = 400,
  // gg -> ZH
  GG2HLL_FWDH = 500,
  // ttH
  TTH_FWDH = 600,
  TTH = 601, // 601, 602, 603, 604, 605 merged into one category
};
static const std::map<STXS_STAGE_1_2_MERGED, std::vector<std::string>>
    STXS1p2_TO_Category_Map = {
        {STXS_STAGE_1_2_MERGED::GG2H_0J_PTH_0_10, {"Untagged_0jet_Pt0To10"}},
        {STXS_STAGE_1_2_MERGED::GG2H_0J_PTH_GT10, {"Untagged_0jet_Pt10To200"}},
        {STXS_STAGE_1_2_MERGED::GG2H_1J_PTH_0_60, {"Untagged_1jet_Pt0To60"}},
        {STXS_STAGE_1_2_MERGED::GG2H_1J_PTH_60_120,
         {"Untagged_1jet_Pt60To120"}},
        {STXS_STAGE_1_2_MERGED::GG2H_1J_PTH_120_200,
         {"Untagged_1jet_Pt120To200"}},
        {STXS_STAGE_1_2_MERGED::GG2H_GE2J_MJJ_0_350_PTH_0_60,
         {"Untagged_2jet_Pt0To60_Mjj0To350"}},
        {STXS_STAGE_1_2_MERGED::GG2H_GE2J_MJJ_0_350_PTH_60_120,
         {"Untagged_2jet_Pt60To120_Mjj0To350"}},
        {STXS_STAGE_1_2_MERGED::GG2H_GE2J_MJJ_0_350_PTH_120_200,
         {"Untagged_2jet_Pt120To200_Mjj0To350"}},
        {STXS_STAGE_1_2_MERGED::GG2H_PTH_GT200, {"Untagged_Pt200above"}},
        {STXS_STAGE_1_2_MERGED::GG2H_GE2J_MJJ_GT350,
         {"Untagged_2jet_Mjj350above"}},
        {STXS_STAGE_1_2_MERGED::QQ2HQQ_rest,
         {"VBF_1jet", "VBF_2jet_Mjj0To350", "VH_hadronic_Mjj0To60",
          "VH_hadronic_Mjj120above"}},
        {STXS_STAGE_1_2_MERGED::QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25,
         {"VBF_2jet_Pt0To200_Mjj350To700_Hjj_pt0To25"}},
        {STXS_STAGE_1_2_MERGED::QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25,
         {"VBF_2jet_Pt0To200_Mjj700above_Hjj_pt0To25"}},
        {STXS_STAGE_1_2_MERGED::QQ2HQQ_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25,
         {"VBF_2jet_Pt0To200_Mjj350To700_Hjj_pt25above",
          "VBF_2jet_Pt0To200_Mjj700above_Hjj_pt25above"}},
        {STXS_STAGE_1_2_MERGED::QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200,
         {"VBF_2jet_Pt200above_Mjj350above"}},
        {STXS_STAGE_1_2_MERGED::QQ2HQQ_GE2J_MJJ_60_120,
         {"VH_hadronic_Mjj60To120"}},
        {STXS_STAGE_1_2_MERGED::VH_lep_PTV_0_150, {"VH_leptonic_Pt0To150"}},
        {STXS_STAGE_1_2_MERGED::VH_lep_PTV_GT150, {"VH_leptonic_Pt150above"}},
        {STXS_STAGE_1_2_MERGED::TTH, {"ttH_hadronic", "ttH_leptonic"}}};

int STXSStage12MergedCat(int cat);

std::string STXSStage12Label(int cat, const std::string &mode);
#endif
