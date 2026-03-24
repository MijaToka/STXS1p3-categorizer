#include "STXS_common.cpp"
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <limits>
#include <map>
#include <set>

constexpr float infty = std::numeric_limits<float>::infinity();

std::set<STXS1> define_untagged_subcategories() {
  std::set<STXS1> untagged_categories;

  STXS1 ggH0j, ggH1j, ggH2jlowMjj, ggH2jhighMjj, ggHhighPt;
  std::set<STXS1> ggH0j_split, ggH1j_split, ggH2jlowMjj_split,
      ggH2jhighMjj_split, ggHhighPt_split;
  ggH0j.Category = STXS0::Untagged;

  // ggH0jet categories
  ggH0j_split = cut_ranges(ggH0j, Pt, {0.f, 10.f, 200.f});
  untagged_categories.insert(ggH0j_split.begin(), ggH0j_split.end());

  // ggH1jet categories
  ggH1j.Category = STXS0::Untagged;
  ggH1j_split = cut_ranges(ggH1j, Pt, {0.f, 60.f, 200.f});
  untagged_categories.insert(ggH1j_split.begin(), ggH1j_split.end());

  // ggH2j_mjj0to350
  ggH2jlowMjj =
      STXS1{.Category = STXS0::Untagged, .Mjj = Range{{"m_jj"}, 0, 350}};
  for (STXS1 pt_cut : cut_ranges(ggH2jlowMjj, Pt, {0.f, 60.f, 200.f})) {
    auto HjjPt_split = cut_ranges(pt_cut, Hjj_pt, {0.f, 25.f, infty});
    ggH2jlowMjj_split.insert(HjjPt_split.begin(), HjjPt_split.end());
  }
  untagged_categories.insert(ggH2jlowMjj_split.begin(),
                             ggH2jlowMjj_split.end());

  // ggH2j_mjj350above
  ggH2jhighMjj = STXS1{.Category = STXS0::Untagged};

  for (STXS1 mjj_cut :
       cut_ranges(ggH2jhighMjj, Mjj, {350.f, 700.f, 1000.f, 1500.f, infty})) {
    std::set<STXS1> pt_split = cut_ranges(mjj_cut, Hjj_pt, {0., 25., infty});

    ggH2jhighMjj_split.insert(pt_split.begin(), pt_split.end());
  }
  untagged_categories.insert(ggH2jlowMjj_split.begin(),
                             ggH2jlowMjj_split.end());

  // ggH_Pt220above
  ggHhighPt.Category = STXS0::Untagged;
  for (STXS1 pt_cut :
       cut_ranges(ggHhighPt, Pt, {200.f, 300.f, 450.f, 650.f, infty})) {
    auto Hj_H_pt_split = cut_ranges(pt_cut, Hj_H_pt, {0.f, 0.15, infty});
    ggHhighPt_split.insert(Hj_H_pt_split.begin(), Hj_H_pt_split.end());
  }
  untagged_categories.insert(ggHhighPt_split.begin(), ggHhighPt_split.end());

  return untagged_categories;
}

std::set<STXS1> define_VBF_categories() {
  std::set<STXS1> VBF_categories;

  // No VBF0j
  // VBF_1jet is simply itself
  STXS1 VBF1j = STXS1{.Category = STXS0::VBF_1jet};
  VBF_categories.insert(VBF1j);

  // VBF_2jet subcategories
  // Low m_jj
  STXS1 VBF2j_lowMjj =
      STXS1{.Category = STXS0::VBF_2jet, .Mjj = Range{{"m_jj"}, 0, 350}};

  for (STXS1 mjj_cut :
       cut_ranges(VBF2j_lowMjj, Mjj, {0.f, 60.f, 120.f, 350.f})) {
    auto Hjj_pt_split = cut_ranges(mjj_cut, Hjj_pt, {0.f, 25.f, infty});
    VBF_categories.insert(Hjj_pt_split.begin(), Hjj_pt_split.end());
  }
  // High m_jj
  STXS1 VBF2j_highMjj = STXS1{.Category = STXS0::VBF_2jet};
  for (STXS1 pt_cut : cut_ranges(VBF2j_highMjj, Pt, {0.f, 200.f, infty})) {
    for (STXS1 mjj_cut :
         cut_ranges(pt_cut, Mjj, {350.f, 700.f, 1000.f, 1500.f, infty})) {
      auto Hjj_pt_split = cut_ranges(mjj_cut, Hjj_pt, {0.f, 25.f, infty});
      VBF_categories.insert(Hjj_pt_split.begin(), Hjj_pt_split.end());
    }
  }
  return VBF_categories;
}

std::set<STXS1> define_categories() {
  std::set<STXS1> all_categories;

  auto untagged_categories = define_untagged_subcategories();
  all_categories.insert(untagged_categories.begin(), untagged_categories.end());
  return all_categories;
}

std::map<STXS1, ROOT::RDF::RNode>
second_categorization(std::map<STXS0, ROOT::RDF::RNode> arr) {

  std::map<STXS1, ROOT::RDF::RNode> output;

  return output;
}
