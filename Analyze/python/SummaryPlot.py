import ROOT
from ROOT import TCanvas, RDataFrame, TLegend, TPad, TH1F, THStack, TLatex, TBox
from numpy.typing import NDArray
from STXSCategorizer.Analyze.UtilFunctions import AMS_2, assert_lumi
import numpy as np
from dataclasses import dataclass


@dataclass
class PadMargins:
    left: float = 0
    right: float = 0
    top: float = 0
    bottom: float = 0

    def applyMargins(self, pad: TPad):
        pad.SetLeftMargin(self.left)
        pad.SetRightMargin(self.right)
        pad.SetTopMargin(self.top)
        pad.SetBottomMargin(self.bottom)


@dataclass
class ObjectBorders:
    left: float = 0
    right: float = 0
    top: float = 0
    bottom: float = 0

    def fromMargins(self, padMargins: PadMargins):
        self.left = padMargins.left
        self.right = 1 - padMargins.right
        self.top = 1 - padMargins.top
        self.bottom = padMargins.bottom


extraHistograms = [totalIdx := -1, sigIdx := -2, sigfracIdx := -3, zvalIdx := -4]


def makePlots(
    df_list: NDArray[RDataFrame],
    modes: list[str],
    categories: list[str],
    signalPerCategory: dict[str, list[str]],
    modeColors: dict[str, int],
    column: str,
    lumi: int = 138,
) -> tuple[THStack, NDArray[TH1F], TLegend]:
    assert len(df_list) == len(categories)
    assert_lumi(lumi)

    hists = np.empty(len(modes) + len(extraHistograms), dtype=TH1F)

    stack = THStack("stack", "stack")

    legend = TLegend(0.01, 0.01, 0.99, 0.99)

    for modeIdx, mode in enumerate(modes):
        hists[modeIdx] = TH1F(mode, mode, len(categories), -0.5, len(categories) - 0.5)
        if mode in modeColors:
            hists[modeIdx].SetFillColor(modeColors[mode])
            hists[modeIdx].SetLineColor(modeColors[mode])
        else:
            hists[modeIdx].SetFillColor(ROOT.kBlack)
            hists[modeIdx].SetLineColor(ROOT.kBlack)
    hists[totalIdx] = TH1F(
        "total", "total", len(categories), -0.5, len(categories) - 0.5
    )
    hists[sigIdx] = TH1F("sig", "sig", len(categories), -0.5, len(categories) - 0.5)
    hists[sigfracIdx] = TH1F(
        "sigfrac", "sigfrac", len(categories), -0.5, len(categories) - 0.5
    )
    hists[zvalIdx] = TH1F("zval", "zval", len(categories), -0.5, len(categories) - 0.5)

    nEventMap = {}
    for modeIdx, mode in enumerate(modes):
        for catIdx, df in enumerate(df_list):
            nEventMap[(catIdx, modeIdx)] = df.Filter(f'{column}=="{mode}"').Sum(
                "EventWeight_lumi{}".format(lumi)
            )

    ROOT.RDF.RunGraphs(list(nEventMap.values()))

    for modeIdx, mode in enumerate(modes):
        for catIdx, category in enumerate(categories):
            nEvents = nEventMap[(catIdx, modeIdx)].GetValue()
            hists[modeIdx].Fill(catIdx, nEvents)
            hists[totalIdx].Fill(catIdx, nEvents)
            if mode in signalPerCategory[category]:
                hists[sigIdx].Fill(catIdx, nEvents)

    for modeIdx, mode in enumerate(modes):
        stack.Add(hists[modeIdx] / hists[totalIdx])
        legend.AddEntry(hists[modeIdx], mode, "f")

    for catIdx, category in enumerate(categories):
        signal = hists[sigIdx].GetBinContent(catIdx + 1)
        total = hists[totalIdx].GetBinContent(catIdx + 1)
        zval = AMS_2(signal, total - signal)
        hists[zvalIdx].Fill(catIdx, zval)
        hists[zvalIdx].GetXaxis().SetBinLabel(catIdx + 1, category)
        hists[sigfracIdx].Fill(catIdx, signal / total)

    return stack, hists, legend


def summaryPlot(
    df_list: NDArray[RDataFrame],
    modes: list[str],
    categories: list[str],
    signalPerCategory: dict[str, list[str]],
    modeColors: dict[str, int],
    column: str,
    lumi: int = 138,
) -> TCanvas:
    # Canvas declaration
    canvas = TCanvas("summary_plot", "STXS Categorization", 1200, 600)
    canvas.SetFillColor(ROOT.kWhite)

    # Left stack
    stackMargins = PadMargins(left=0.2, right=0.01, top=0.2, bottom=0.1)
    plotBorders = ObjectBorders()
    plotBorders.fromMargins(stackMargins)

    stackPad = TPad(
        "stackPad", "stackPad", 0, 0, splitCoord := 0.8, topMargin := 0.95
    )  # Leave space for CMS and lumi header
    stackMargins.applyMargins(stackPad)
    stackPad.Draw()

    # Right table
    tablePad = TPad(
        "tablePad", "tablePad", splitCoord, 0, 1 - stackMargins.right, topMargin
    )
    tablePad.SetLeftMargin(0)
    tablePad.SetRightMargin(0)
    tablePad.SetTopMargin(0)
    tablePad.SetBottomMargin(stackMargins.bottom)
    tablePad.Draw()

    canvas._stackPad = stackPad
    canvas._tablePad = tablePad

    ## Calculate the plots
    stack, hists, legend = makePlots(
        df_list, modes, categories, signalPerCategory, modeColors, column, lumi
    )
    canvas._hists = hists

    totalHist = hists[totalIdx]
    signalFractionHist = hists[sigfracIdx]
    zvalHist = hists[zvalIdx]

    ## Build the stack
    stackPad.cd()

    stack.Draw("HBAR")
    canvas._stack = stack
    stack.SetMinimum(0)
    stack.SetMaximum(1)
    for catIdx, category in enumerate(categories):
        stack.GetXaxis().SetBinLabel(catIdx + 1, category)
        stack.GetXaxis().ChangeLabel(catIdx + 1, 40)

    xAx = stack.GetXaxis()
    xAx.SetTitle("Reconstructed event category")
    xAx.SetLabelSize(0.02)
    xAx.SetTitleOffset(2.2)
    xAx.SetTitleSize(0.035)

    yAx = stack.GetYaxis()
    yAx.SetTitle("Signal production bin fraction")
    yAx.SetLabelSize(0.03)
    yAx.SetTitleOffset(1.1)
    yAx.SetTitleSize(0.03)

    ## Legend configuration
    targetNRows = 6
    nCols = int(np.ceil(len(modes) / targetNRows))
    legend.SetNColumns(nCols)
    legend.SetBorderSize(1)
    legend.SetMargin(0.15)
    legend.SetTextSize(0)

    legend.SetX1(stackMargins.left)
    legend.SetY1(1 - stackMargins.top + 0.01)
    legend.SetX2(1 - stackMargins.right)
    legend.SetY2(1)

    legend.Draw()
    canvas._legend = legend

    ## Create the table
    tablePad.cd()

    latex = TLatex()
    latex.SetTextFont(42)

    canvas._latex = latex

    columnText = [r"N_{events}", r"#frac{S}{S+B}", "#sigma-val"]
    nColumns = len(columnText)
    columnBorders = np.linspace(0, 1, nColumns + 1)
    centerShift = 0.6
    columnCenters = (
        centerShift * columnBorders[:-1] + (1 - centerShift) * columnBorders[1:]
    )

    outerBox = TBox(0, plotBorders.bottom, 1, 1)
    outerBox.SetFillColor(ROOT.kWhite)
    outerBox.SetLineColor(ROOT.kBlack)
    outerBox.SetLineWidth(2)
    outerBox.Draw("l")

    canvas._outerBox = outerBox

    rowHeight = (plotBorders.top - plotBorders.bottom) / len(categories)

    # Values looped from bottom up
    for catIdx, category in enumerate(categories):
        nEvents = totalHist.GetBinContent(catIdx + 1)
        signalFraction = signalFractionHist.GetBinContent(catIdx + 1)
        zval = zvalHist.GetBinContent(catIdx + 1)

        rowValues = [f"{nEvents:.3f}", f"{100*signalFraction:.1f}%", f"{zval:.2f}"]
        assert len(rowValues) == nColumns

        for i in range(nColumns):
            latex.DrawLatexNDC(
                columnCenters[i],
                plotBorders.bottom + rowHeight * (catIdx + 0.4),
                rowValues[i],
            )

    # Header
    for i in range(nColumns):
        latex.DrawLatexNDC(
            columnCenters[i],
            (1 - stackMargins.top) + 0.5 * (stackMargins.top),
            columnText[i],
        )

    return canvas
