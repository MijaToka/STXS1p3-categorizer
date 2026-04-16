import numpy as np
import ROOT
from ROOT import RDataFrame
from typing import TypedDict
from numpy.typing import NDArray

signalProductionModes = [
    "ggH125",
    "VBFH125",
    "ZH125",
    "WplusH125",
    "WminusH125",
    "ttH125",
]

bkgProductionModes = [
    "ZZTo4l",
    "WWZ",
    "ZZZ",
    "ggTo4mu_Contin_MCFM701",
    "ggTo2e2mu_Contin_MCFM701",
    "ggTo2mu2tau_Contin_MCFM701",
    "TTZZ",
    "ggTo2e2tau_Contin_MCFM701",
    "TTWW",
    "ggTo4tau_Contin_MCFM701",
    "ggTo4e_Contin_MCFM701",
]


class PlotsKwarg(TypedDict, total=False):
    df_list: NDArray[RDataFrame]
    modes: list[str]
    categories: list[str]
    signalPerCategory: dict[str, list[str]]
    modeColors: dict[str, int]
    column: str
    lumi: int


def setupModes(config: PlotsKwarg, step: int) -> PlotsKwarg:
    """
    Set's up the modes, categories and modeColors of the summaryPlot kwargs
    """
    assert step in [0, 2], f"Step {step} not valid, must be 0 or 2"

    if step == 0:
        targetsStage0 = {
            "WplusH125": ROOT.kGreen + 1,
            "WminusH125": ROOT.kGreen + 2,
            "ZH125": ROOT.kGreen,
            "ggH125": ROOT.kAzure + 1,
            "VBFH125": ROOT.kOrange + 7,
            "ttH125": ROOT.kViolet,
        }
        for mode in bkgProductionModes:
            targetsStage0[mode] = ROOT.kBlack

        STXS0 = [
            "Untagged",
            "VBF_1jet",
            "VBF_2jet",
            "VH_leptonic",
            "VH_hadronic",
            "ttH_leptonic",
            "ttH_hadronic",
        ]

        signalPerCategoryStep0 = {
            "ttH_hadronic": ["ttH125"],
            "ttH_leptonic": ["ttH125"],
            "VH_hadronic": ["WminusH125", "WplusH125", "ZH125"],
            "VH_leptonic": ["WminusH125", "WplusH125", "ZH125"],
            "VBF_2jet": ["VBFH125"],
            "VBF_1jet": ["VBFH125"],
            "Untagged": ["ggH125"],
        }
        config["modes"] = list(targetsStage0.keys())
        config["modeColors"] = targetsStage0
        config["categories"] = STXS0
        config["signalPerCategory"] = signalPerCategoryStep0
        config["column"] = "production_mode"

    elif step == 2:
        STXS1p2 = [
            "Untagged_0jet_Pt0To10",  # Start ggH
            "Untagged_0jet_Pt10To200",
            "Untagged_1jet_Pt0To60",
            "Untagged_1jet_Pt60To120",
            "Untagged_1jet_Pt120To200",
            "Untagged_2jet_Pt0To60_Mjj0To350",
            "Untagged_2jet_Pt60To120_Mjj0To350",
            "Untagged_2jet_Pt120To200_Mjj0To350",
            "Untagged_Pt200above",
            "Untagged_2jet_Mjj350above",  # End ggH
            "VBF_1jet",  # Start EW
            "VBF_2jet_Pt0To200_Mjj350To700_Hjj_pt0To25",
            "VBF_2jet_Pt0To200_Mjj700above_Hjj_pt0To25",
            "VBF_2jet_Pt0To200_Mjj350To700_Hjj_pt25above",
            "VBF_2jet_Pt0To200_Mjj700above_Hjj_pt25above",
            "VBF_2jet_Pt200above_Mjj350above",
            "VBF_2jet_Mjj0To350",
            "VH_hadronic_Mjj60To120",
            "VH_hadronic_Mjj0To60",
            "VH_hadronic_Mjj120above",  # End EW
            "VH_leptonic_Pt0To150",  # Start VH_lep
            "VH_leptonic_Pt150above",  # End VH_lep
            "ttH_leptonic",
            "ttH_hadronic",
        ]

        targetsStage1p2 = {
            "GG2H_0J_PTH_0_10": ROOT.kRed + 4,
            "GG2H_0J_PTH_GT10": ROOT.kRed + 3,
            "GG2H_1J_PTH_0_60": ROOT.kRed + 2,
            "GG2H_1J_PTH_60_120": ROOT.kRed + 1,
            "GG2H_1J_PTH_120_200": ROOT.kRed,
            "GG2H_GE2J_MJJ_0_350_PTH_0_60": ROOT.kRed - 1,
            "GG2H_GE2J_MJJ_0_350_PTH_60_120": ROOT.kRed - 2,
            "GG2H_GE2J_MJJ_0_350_PTH_120_200": ROOT.kRed - 3,
            "GG2H_GE2J_MJJ_GT350": ROOT.kRed - 4,
            "GG2H_PTH_GT200": ROOT.kRed - 5,
            "GG2H_FWDH": ROOT.kRed - 6,
            "GG2HLL_FWDH": ROOT.kRed - 7,
            "VBF_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25": ROOT.kMagenta + 4,
            "VBF_GE2J_MJJ_60_120": ROOT.kMagenta + 3,
            "VBF_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25": ROOT.kMagenta + 2,
            "VBF_GE2J_MJJ_GT350_PTH_GT200": ROOT.kMagenta + 1,
            "VBF_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25": ROOT.kMagenta,
            "VBF_rest": ROOT.kMagenta - 1,
            "VBF_FWDH": ROOT.kMagenta - 2,
            "WminusH_lep_PTV_0_150": ROOT.kYellow + 4,
            "WminusH_lep_PTV_GT150": ROOT.kYellow + 3,
            "WminushadH_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25": ROOT.kMagenta + 2,
            "WminushadH_GE2J_MJJ_60_120": ROOT.kMagenta + 1,
            "WminushadH_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25": ROOT.kMagenta,
            "WminushadH_GE2J_MJJ_GT350_PTH_GT200": ROOT.kMagenta - 1,
            "WminushadH_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25": ROOT.kMagenta - 2,
            "WminushadH_rest": ROOT.kMagenta - 3,
            "WminushadH_FWDH": ROOT.kMagenta - 4,
            "WplusH_lep_PTV_0_150": ROOT.kYellow + 2,
            "WplusH_lep_PTV_GT150": ROOT.kYellow,
            "WplushadH_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25": ROOT.kViolet + 2,
            "WplushadH_GE2J_MJJ_60_120": ROOT.kViolet + 1,
            "WplushadH_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25": ROOT.kViolet,
            "WplushadH_GE2J_MJJ_GT350_PTH_GT200": ROOT.kViolet - 1,
            "WplushadH_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25": ROOT.kViolet - 2,
            "WplushadH_rest": ROOT.kViolet - 3,
            "WplushadH_FWDH": ROOT.kViolet - 4,
            "ZH_lep_PTV_0_150": ROOT.kYellow - 1,
            "ZH_lep_PTV_GT150": ROOT.kYellow - 2,
            "ZhadH_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25": ROOT.kBlue + 2,
            "ZhadH_GE2J_MJJ_60_120": ROOT.kBlue + 1,
            "ZhadH_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25": ROOT.kBlue,
            "ZhadH_GE2J_MJJ_GT350_PTH_GT200": ROOT.kBlue - 1,
            "ZhadH_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25": ROOT.kBlue - 2,
            "ZhadH_rest": ROOT.kBlue - 3,
            "ZhadH_FWDH": ROOT.kBlue - 4,
            "TTH": ROOT.kOrange + 7,
            "TTH_FWDH": ROOT.kOrange + 9,
            "UNKNOWN": ROOT.kBlack,
        }

        signalPerCategoryStep1p2 = {
            "Untagged_0jet_Pt0To10": ["GG2H_0J_PTH_0_10"],
            "Untagged_0jet_Pt10To200": ["GG2H_0J_PTH_GT10"],
            "Untagged_1jet_Pt0To60": ["GG2H_1J_PTH_0_60"],
            "Untagged_1jet_Pt60To120": ["GG2H_1J_PTH_60_120"],
            "Untagged_1jet_Pt120To200": ["GG2H_1J_PTH_120_200"],
            "Untagged_2jet_Pt0To60_Mjj0To350": ["GG2H_GE2J_MJJ_0_350_PTH_0_60"],
            "Untagged_2jet_Pt60To120_Mjj0To350": ["GG2H_GE2J_MJJ_0_350_PTH_60_120"],
            "Untagged_2jet_Pt120To200_Mjj0To350": ["GG2H_GE2J_MJJ_0_350_PTH_120_200"],
            "Untagged_Pt200above": ["GG2H_PTH_GT200"],
            "Untagged_2jet_Mjj350above": ["GG2H_GE2J_MJJ_GT350"],
            "VBF_1jet": ["VBF_rest"],
            "VBF_2jet_Pt0To200_Mjj350To700_Hjj_pt0To25": [
                "VBF_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25"
            ],
            "VBF_2jet_Pt0To200_Mjj700above_Hjj_pt0To25": [
                "VBF_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25"
            ],
            "VBF_2jet_Pt0To200_Mjj350To700_Hjj_pt25above": [
                "VBF_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25"
            ],
            "VBF_2jet_Pt0To200_Mjj700above_Hjj_pt25above": [
                "VBF_GE2J_MJJ_GT350_PTH_0_200_PTHJJ_GT25"
            ],
            "VBF_2jet_Pt200above_Mjj350above": ["VBF_GE2J_MJJ_GT350_PTH_GT200"],
            "VBF_2jet_Mjj0To350": ["VBF_rest"],
            "VH_hadronic_Mjj60To120": [
                "WminushadH_GE2J_MJJ_60_120",
                "WplushadH_GE2J_MJJ_60_120",
                "ZhadH_GE2J_MJJ_60_120",
            ],
            "VH_hadronic_Mjj0To60": ["WminushadH_rest", "WplushadH_rest", "ZhadH_rest"],
            "VH_hadronic_Mjj120above": [
                "WminushadH_rest",
                "WplushadH_rest",
                "ZhadH_rest",
            ],
            "VH_leptonic_Pt0To150": [
                "WminusH_lep_PTV_0_150",
                "WplusH_lep_PTV_0_150",
                "ZH_lep_PTV_0_150",
            ],
            "VH_leptonic_Pt150above": [
                "WminusH_lep_PTV_GT150",
                "WplusH_lep_PTV_GT150",
                "ZH_lep_PTV_GT150",
            ],
            "ttH_leptonic": ["TTH"],
            "ttH_hadronic": ["TTH"],
        }

        config["modes"] = list(targetsStage1p2.keys())
        config["modeColors"] = targetsStage1p2
        config["categories"] = STXS1p2
        config["signalPerCategory"] = signalPerCategoryStep1p2
        config["column"] = "HTXS_stage1_2_cat_pTjet30GeV_label"

    return config


def setupDataFrames(
    config: PlotsKwarg, step: int, irrBkg: bool = False, dir: str = "."
) -> PlotsKwarg:
    """
    Initializes the dataframes found in dir/{modes}/{categorization step}/{catergories}.root
    and saves them to config[df_list]
    """
    assert not config.get("categories") is None
    assert step in [0, 2, 3]

    stepName = {
        0: "first_categorization",
        2: "second_categorization_version2",
        3: "second_categorization_version3",
    }

    modes = signalProductionModes + bkgProductionModes * irrBkg

    dataframes_list = np.empty(
        len(config.get("categories", [])), dtype=type(RDataFrame)
    )
    for catIdx, category in enumerate(config.get("categories", [])):
        dataframes_list[catIdx] = RDataFrame(
            "Events",
            [f"{dir}/{mode}/{stepName[step]}/{category}.root" for mode in modes],
        )
    config["df_list"] = dataframes_list
    return config


def setupPlot(step: int, irrBkg: bool, dir: str = ".", lumi: int = 138) -> PlotsKwarg:

    config = PlotsKwarg()

    config["lumi"] = lumi
    config = setupModes(config, step)
    config = setupDataFrames(config, step, irrBkg, dir)

    return config
