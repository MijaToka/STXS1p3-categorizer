#include <RtypesCore.h>
#include <optional>
#include <set>
#include <string>
#include <vector>

enum class STXS0 {
  VBF_2jet,
  VH_hadronic,
  VH_leptonic,
  ttH_hadronic,
  ttH_leptonic,
  VBF_1jet,
  Untagged
};

enum Category { Pt, Mjj, Hjj_pt, Hj_H_pt, deltaPhi_jj, nJets };

struct Range {
  std::vector<std::string> columns;
  float lower;
  float upper;
};

struct numberJets {
  int nJet;
  bool above;
};

struct STXS1 {
  STXS0 Category;
  std::optional<Range> Pt, Mjj, Hjj_pt, Hj_H_pt, deltaPhi_jj;
  std::optional<numberJets> nJets;
};

std::set<STXS1> cut_ranges(STXS1 prevSTXS1, const Category column,
                           std::vector<Float_t> ranges) {
  std::set<STXS1> output;
  for (int i = 1; i < ranges.size(); i++) {
    STXS1 newCategory = prevSTXS1;
    switch (column) {
    case Pt:
      newCategory.Pt =
          Range{{"ZZCand_pt", "bestCandIdx"}, ranges[i - 1], ranges[i]};
      break;
    case Mjj:
      newCategory.Mjj = Range{{"m_jj"}, ranges[i - 1], ranges[i]};
      break;
    case Hjj_pt:
      newCategory.Hjj_pt = Range{{"ZZjj_pt"}, ranges[i - 1], ranges[i]};
      break;
    case Hj_H_pt:
      newCategory.Hj_H_pt = Range{
          {"ZZj_pt", "ZZCand_pt", "bestCandIdx"}, ranges[i - 1], ranges[i]};
      break;
    case deltaPhi_jj:
      newCategory.deltaPhi_jj =
          Range{{"deltaPhi_jj"}, ranges[i - 1], ranges[i]};
      break;
    case nJets:
      break;
    }
    output.insert(newCategory);
  }
  return output;
};
