#ifndef VARIABLES_STXS1_2_H
#define VARIABLES_STXS1_2_H
#include <map>
#include <string>

inline const std::map<std::string, char> variablesSTXS1p2 = {
    // Discriminants
    {"DVBF2j_ME", 'F'},
    {"DVBF1j_ME", 'F'},
    {"DWHh_ME", 'F'},
    {"DZHh_ME", 'F'},
    {"ZZCand_KD_bestCand", 'F'},

    // ZZ candidate features
    {"ZZCand_pt_bestCand", 'F'},
    {"ZZCand_eta_bestCand", 'F'},
    {"ZZCand_phi_bestCand", 'F'},
    {"ZZCand_mass_bestCand", 'F'},
    {"ZZCand_costheta1_bestCand", 'F'},
    {"ZZCand_costheta2_bestCand", 'F'},
    {"ZZCand_costhetastar_bestCand", 'F'},
    {"ZZCand_Phi1_bestCand", 'F'},
    {"ZZCand_nExtraLep_bestCand", 'I'},

    // Jet features
    {"JetLeading_pt", 'F'},
    {"JetLeading_eta", 'F'},
    {"JetLeading_mass", 'F'},
    {"JetLeading_phi", 'F'},
    {"JetLeading_btag", 'F'},
    {"JetSubleading_pt", 'F'},
    {"JetSubleading_eta", 'F'},
    {"JetSubleading_mass", 'F'},
    {"JetSubleading_phi", 'F'},
    {"JetSubleading_btag", 'F'},

    {"nCleanedJetsPt30", 'I'},
    {"nBtagged_filtered", 'I'},

    // Dijet features
    {"deltaEta_jj", 'F'},
    {"deltaPhi_jj", 'F'},
    {"m_jj", 'F'},

    // Lepton features (primary 4 leptons)
    {"LepPt_0", 'F'},
    {"LepPt_1", 'F'},
    {"LepPt_2", 'F'},
    {"LepPt_3", 'F'},
    {"LepEta_0", 'F'},
    {"LepEta_1", 'F'},
    {"LepEta_2", 'F'},
    {"LepEta_3", 'F'},
    {"LepPhi_0", 'F'},
    {"LepPhi_1", 'F'},
    {"LepPhi_2", 'F'},
    {"LepPhi_3", 'F'},
    {"LepPdgId_0", 'I'},
    {"LepPdgId_1", 'I'},
    {"LepPdgId_2", 'I'},
    {"LepPdgId_3", 'I'},

    // Extra lepton features
    {"LepPt_4", 'F'},
    {"LepPt_5", 'F'},
    {"LepEta_4", 'F'},
    {"LepEta_5", 'F'},
    {"LepPhi_4", 'F'},
    {"LepPhi_5", 'F'},
    {"LepPdgId_4", 'I'},
    {"LepPdgId_5", 'I'},

    // MET
    {"PFMET_pt", 'F'},
};

#endif
