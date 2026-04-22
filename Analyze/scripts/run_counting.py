#!/usr/bin/env python3
from argparse import ArgumentParser, Namespace


def parseArgs():
    parser = ArgumentParser(
        prog="run_counting",
        description="This program counts the number of weighted events in the output files computed from STXSCategorizer, produces a summary plot, and generates data cards from the",
    )
    parser.add_argument("--background", action="store_true")
    parser.add_argument("--directory", "-d")
    parser.add_argument("--output_dir", "-o")
    parser.add_argument("--version", "-v",type=int)
    parser.add_argument("--plot", action="store_true")
    parser.add_argument("--lumi","-l",default=138,type=int)

    return parser.parse_args()


def main(args: Namespace) -> None:
    kwarg_setupPlots = {
        "step": int(args.version),
        "irrBkg": args.background,
        "dir": args.directory,
        "lumi": args.lumi,
    }

    from STXSCategorizer.Analyze.SetupPlots import setupPlot

    kwarg_summaryPlots = setupPlot(**kwarg_setupPlots)

    from STXSCategorizer.Analyze.SummaryPlot import summaryPlot
    
    canvas = summaryPlot(**kwarg_summaryPlots)
    if args.plot:
        fileName = "STXS" + ("0" if args.version == 0 else f"1p{args.version}")
        canvas.SaveAs(f"{args.output_dir}/{fileName}.pdf")
    
    from STXSCategorizer.Analyze.DataCards import data_cards_from_summaryPlot
    
    data_cards_from_summaryPlot(kwarg_summaryPlots, canvas, args.output_dir, args.version)

    return


if __name__ == "__main__":
    args = parseArgs()
    main(args)
