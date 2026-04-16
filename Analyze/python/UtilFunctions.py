from numpy import sqrt, log
import numpy as np
from numpy.typing import NDArray
from ROOT import RDataFrame, TCanvas, TLegend, TH1F, THStack
import ROOT


def AMS_2(signal: float, background: float, regulator: float = 0) -> float:
    if background + regulator == 0:
        return 0
    return float(
        sqrt(
            2
            * (
                (signal + background + regulator)
                * log(1 + signal / (background + regulator))
                - signal
            )
        )
    )


def assert_lumi(lumi: int) -> None:
    assert lumi in [
        9,
        18,
        138,
        250,
        300,
        350,
    ], f"Invalid lumi value: {lumi}. Must be a value in [9,18,138,250,300,350]"


def run_plots(
    df_list: list[RDataFrame],
    modes: list[str],
    categories: list[str],
    signalPerCategory: dict[str, list[str]],
    modeColors: dict[str, int],
    column: str,
    lumi: int = 138,
) -> tuple[TCanvas, list[THStack], NDArray[TH1F], TLegend]:
    assert len(df_list) == len(categories)
    assert_lumi(lumi)

    if ROOT.gROOT.FindObject("canvas"):
        ROOT.gROOT.FindObject("canvas").Delete()
    canvas = TCanvas("canvas", "canvas", 1200, 800)

    # Initialize objects
    extraHistograms = [totalIdx := -1, sigIdx := -2, bkgIdx := -3, zvalIdx := -4]
    hists = np.empty(len(modes) + len(extraHistograms), dtype=TH1F)

    stack_fraction = THStack(
        "stack_fraction", ";Reconstructed event category;Signal production bin fraction"
    )
    stack_absolute = THStack(
        "stack_absolute", ";Reconstructed event category;Signal production bin"
    )
    stack_SB = THStack(
        "stack_SoB", ";Reconstructed event category;Signal vs background bin fraction"
    )

    legend = TLegend(0.7, 0.7, 0.9, 0.85)
    legend.SetNColumns(int(np.ceil(len(modes) / 5)))

    # Set colors to the histograms
    for modeIdx, mode in enumerate(modes):
        hists[modeIdx] = TH1F(
            mode + "_hist", mode, len(categories), -0.5, len(categories) - 0.5
        )
        if mode in modeColors:
            hists[modeIdx].SetFillColor(modeColors[mode])
            hists[modeIdx].SetLineColor(modeColors[mode])
        else:
            hists[modeIdx].SetFillColor(ROOT.kBlack)
            hists[modeIdx].SetLineColor(ROOT.kBlack)
    hists[totalIdx] = TH1F(
        "total_hist", "total", len(categories), -0.5, len(categories) - 0.5
    )
    hists[zvalIdx] = TH1F(
        "zval_hist",
        "Significance;Category;Significance value",
        len(categories),
        -0.5,
        len(categories) - 0.5,
    )
    hists[zvalIdx].SetFillColor(ROOT.kAzure + 9)

    hists[sigIdx] = TH1F(
        "sig_hist", "signal", len(categories), -0.5, len(categories) - 0.5
    )
    hists[sigIdx].SetFillColor(ROOT.kBlue)
    hists[sigIdx].SetLineColor(ROOT.kBlue)

    hists[bkgIdx] = TH1F(
        "bkg_hist", "background", len(categories), -0.5, len(categories) - 0.5
    )
    hists[bkgIdx].SetFillColor(ROOT.kRed)
    hists[bkgIdx].SetLineColor(ROOT.kRed)

    # Set up the values to calculate
    nEvent_map = {}
    for modeIdx, mode in enumerate(modes):
        for catIdx, df in enumerate(df_list):
            nEvent_map[(catIdx, modeIdx)] = df.Filter(f'{column} == "{mode}"').Sum(
                "EventWeight_lumi{}".format(lumi)
            )

    # Calculate all the values
    ROOT.RDF.RunGraphs(list(nEvent_map.values()))

    # Fill the histograms
    for modeIdx, mode in enumerate(modes):
        for catIdx, category in enumerate(categories):
            nEvents = nEvent_map[(catIdx, modeIdx)].GetValue()
            hists[modeIdx].Fill(catIdx, nEvents)
            hists[totalIdx].Fill(catIdx, nEvents)
            if mode in signalPerCategory[category]:
                hists[sigIdx].Fill(catIdx, nEvents)
            else:
                hists[bkgIdx].Fill(catIdx, nEvents)

    # Stack the histograms
    for modeIdx, mode in enumerate(modes):
        stack_fraction.Add(hists[modeIdx] / hists[totalIdx])
        stack_absolute.Add(hists[modeIdx])
        legend.AddEntry(hists[modeIdx], mode, "f")
    stack_SB.Add(hists[sigIdx] / hists[totalIdx])
    stack_SB.Add(hists[bkgIdx] / hists[totalIdx])

    # Calculate the Significance
    for catIdx, category in enumerate(categories):
        signal = hists[sigIdx].GetBinContent(catIdx + 1)
        background = hists[bkgIdx].GetBinContent(catIdx + 1)
        zval = AMS_2(signal, background)
        hists[zvalIdx].Fill(catIdx, zval)
        hists[zvalIdx].GetXaxis().SetBinLabel(catIdx + 1, category)

    # Draw
    canvas.Divide(2, 2)
    for i, stack in enumerate(stacks := [stack_fraction, stack_absolute, stack_SB]):
        canvas.cd(i + 1)
        stack.Draw("HBAR STACK")
        stack.SetMinimum(0)
        if i != 1:
            stack.SetMaximum(1)
        for i, label in enumerate(categories):
            stack.GetXaxis().SetBinLabel(i + 1, label)
        if stack != stack_SB:
            legend.Draw()

    canvas.cd(4)
    hists[zvalIdx].Draw("HBAR")
    hists[zvalIdx].SetMinimum(0)

    canvas.Draw()
    return canvas, stacks, hists, legend


def calculate_significance(
    df: RDataFrame,
    signalModes: list[str],
    modes: list[str],
    column: str,
    lumi: int = 138,
) -> float:
    assert_lumi(lumi)
    nEvent_map = {}
    for mode in modes:
        nEvent_map[mode] = df.Filter(f'{column} == "{mode}"').Sum(
            f"EventWeight_lumi{lumi}"
        )

    ROOT.RDF.RunGraphs(list(nEvent_map.values()))

    signal = 0
    background = 0
    for mode in modes:
        nEvents = nEvent_map[mode].GetValue()
        if mode in signalModes:
            signal += nEvents
        else:
            background += nEvents

    return AMS_2(signal, background)
