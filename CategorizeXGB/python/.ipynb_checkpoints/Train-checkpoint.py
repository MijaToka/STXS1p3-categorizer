from turtle import color
from typing import Any, cast
import argparse
import pandas as pd
import numpy as np
from numpy.typing import NDArray
from scipy.special import eval_sh_legendre
from scipy.stats import nchypergeom_wallenius
from sklearn.preprocessing import LabelEncoder, label_binarize
from sklearn.metrics import accuracy_score, classification_report, roc_curve, auc
import uproot
import uproot.reading


def parseArgs():
    parser = argparse.ArgumentParser(prog="Train.py")
    parser.add_argument("--nTree", type=int, default=100)
    parser.add_argument("--maxDepth", type=int, default=5)
    parser.add_argument("--learningRate", type=float, default=0.1)
    parser.add_argument("--evalMetric", type=str, default="mlogloss")
    parser.add_argument("--subsample", type=float, default=0.3)
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.add_argument("--earlyStopRound", type=int, default=5)

    return parser.parse_args()


def main(args) -> None:
    filename: str = "snapshot.root"
    directories: list[str] = [
        f"{channel}H125" for channel in ["gg", "VBF", "Wminus", "Wplus", "Z", "tt"]
    ]
    treename: str = "Events"
    STXS0_names = {
        0: "UNKNOWN",
        10: "GG2H_FWDH",
        11: "GG2H",
        20: "VBF_FWDH",
        21: "VBF",
        22: "VH2HQQ_FWDH",
        23: "VH2HQQ",
        30: "QQ2HLNU_FWDH",
        31: "QQ2HLNU",
        40: "QQ2HLL_FWDH",
        41: "QQ2HLL",
        50: "GG2HLL_FWDH",
        51: "GG2HLL",
        60: "TTH_FWDH",
        61: "TTH",
        70: "BBH_FWDH",
        71: "BBH",
        80: "TH_FWDH",
        81: "TH",
    }
    VARS: list[str] = [
        # Discriminant
        "DVBF2j_ME",
        "DVBF1j_ME",
        "DWHh_ME",
        "DZHh_ME",
        "ZZCand_KD_bestCand",
        # ZZ candidate features
        "ZZCand_pt_bestCand",
        "ZZCand_eta_bestCand",
        "ZZCand_phi_bestCand",
        "ZZCand_mass_bestCand",
        "ZZCand_costheta1_bestCand",
        "ZZCand_costheta2_bestCand",
        "ZZCand_costhetastar_bestCand",
        "ZZCand_Phi1_bestCand",
        "ZZCand_nExtraLep_bestCand",
        # Jet features
        "JetLeading_pt",
        "JetLeading_eta",
        "JetLeading_mass",
        "JetLeading_phi",
        "JetLeading_btag",
        "JetSubleading_pt",
        "JetSubleading_eta",
        "JetSubleading_mass",
        "JetSubleading_phi",
        "JetSubleading_btag",
        "nCleanedJetsPt30",
        "nBtagged_filtered",
        # Dijet features
        "deltaEta_jj",
        "deltaPhi_jj",
        "m_jj",
        # Lepton features (primary 4 leptons)
        "LepPt_0",
        "LepPt_1",
        "LepPt_2",
        "LepPt_3",
        "LepEta_0",
        "LepEta_1",
        "LepEta_2",
        "LepEta_3",
        "LepPhi_0",
        "LepPhi_1",
        "LepPhi_2",
        "LepPhi_3",
        "LepPdgId_0",
        "LepPdgId_1",
        "LepPdgId_2",
        "LepPdgId_3",
        # Extra lepton features
        "LepPt_4",
        "LepPt_5",
        "LepEta_4",
        "LepEta_5",
        "LepPhi_4",
        "LepPhi_5",
        "LepPdgId_4",
        "LepPdgId_5",
        # MET
        "PFMET_pt",
    ]

    DISC_COL: str = "HTXS_stage_0"
    dataFrames: list[pd.DataFrame] = [
        uproot.open(f"output/train/{dir}/{filename}")[treename].arrays(
            VARS + [DISC_COL], library="pd"
        )
        for dir in directories
    ]

    df: pd.DataFrame = pd.concat(dataFrames, ignore_index=True)
    isFWDHMask = np.isin(df[DISC_COL], [10, 20, 22, 30, 40, 50, 60, 70, 80])
    df = df[~isFWDHMask]
    dfX: pd.DataFrame = df.drop(DISC_COL, axis=1)
    dfy: pd.Series = pd.Series(df[DISC_COL])

    le: LabelEncoder = LabelEncoder()
    y_encoded: NDArray[np.int_] = cast(NDArray[np.int_], le.fit_transform(dfy))

    from sklearn.model_selection import train_test_split

    X_train, X_test, y_train, y_test = train_test_split(
        dfX, y_encoded, test_size=0.4, random_state=42, stratify=y_encoded
    )

    from xgboost import XGBClassifier

    model = XGBClassifier(
        num_class=len(le.classes_),
        eval_metric=args.evalMetric,
        max_depth=args.maxDepth,
        learning_rate=args.learningRate,
        n_estimators=args.nTree,
        subsample=args.subsample,
        objective="multi:softprob",
        # early_stopping_rounds=args.earlyStopRound,
    )

    trainedModel = model.fit(
        X_train,
        y_train,
        eval_set=[(X_train, y_train), (X_test, y_test)],
        verbose=args.verbose,
    )

    y_pred = model.predict(X_test)
    y_prob = model.predict_proba(X_test)
    print("Accuracy:", accuracy_score(y_test, y_pred))
    class_names = [STXS0_names.get(int(cls)) for cls in le.classes_]
    print(
        "Classification Report:\n",
        classification_report(y_test, y_pred, target_names=class_names),
    )
    print("Predicted class labels:", y_pred[:10])
    print("Actual class labels:   ", y_test[:10])

    print("Predicted probabilities for first 5 samples:\n", y_prob[:10])

    from xgboost import plot_importance
    import matplotlib.pyplot as plt

    evals_result = trainedModel.evals_result()

    fig = plt.figure(figsize=(5, 4))
    for i, label in enumerate(["Train", "Test"]):
        plt.plot(
            evals_result[f"validation_{i}"][args.evalMetric], label=label, linewidth=2
        )

    plt.xlabel("Epoch")
    plt.ylabel("Loss")
    plt.legend()
    plt.show()

    # Plot ROC

    plot_importance(model, importance_type="weight", xlabel="F-score")
    plt.title("Feature Importance (by F-score)")
    plt.show()

    nClasses = len(le.classes_)

    y_test_bin = label_binarize(y_test, classes=range(nClasses))
    for i in range(nClasses):
        plt.subplot(4, (nClasses - 1) // 2 + 1, i + 1)
        signal_scores = y_prob[y_test == i, i]
        bkg_scores = y_prob[y_test != i, i]

        plt.hist(
            signal_scores,
            bins=100,
            label=f"Signal ({class_names[i]})",
            density=True,
            alpha=0.7,
        )
        plt.hist(
            bkg_scores,
            bins=100,
            label=f"Background ({class_names[i]})",
            density=True,
            alpha=0.7,
        )
        plt.xlabel("BDT Score")
        plt.ylabel("Normalized event count")
        plt.legend()

        plt.subplot(4, (nClasses - 1) // 2 + 1, nClasses + i + 1)
        fpr, tpr, _ = roc_curve(y_test_bin[:, i], y_prob[:, i])
        roc_auc = auc(fpr, tpr)
        print("Category:", class_names[i])
        print("fpr:", fpr)
        print("tpr:", tpr)
        plt.plot(fpr, tpr, color="blue", label=f"ROC (AUC = {roc_auc:.3f})")
        plt.plot([0, 1], [0, 1], "k--", label="Random")

        plt.xlim([0, 1])
        plt.ylim([0, 1.05])
        plt.xlabel("False positive rate")
        plt.ylabel("True positive rate")
        plt.title(f"{class_names[i]} ROC curve")
        plt.legend()
    plt.show()

    import ROOT

    booster = model.get_booster()
    setattr(booster, "objective", str(model.objective))
    ROOT.TMVA.Experimental.SaveXGBoost(
        booster,
        "multiclassXGB",
        "multiclassXGB_model.root",
        num_inputs=len(VARS),
    )


if __name__ == "__main__":
    args = parseArgs()
    main(args)
