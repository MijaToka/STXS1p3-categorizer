from ROOT import TCanvas
from dataclasses import dataclass, field
from typing import IO,Optional
from numpy import round
from STXSCategorizer.Analyze.SetupPlots import PlotsKwarg


def write_header(
    file: IO[str],
    nbin: int,
    nprocesses: Optional[int] = None,
    nnuisanceparams: Optional[int] = None,
):
    print(f"imax {nbin} number of bins", file=file)
    print(
        f"jmax {nprocesses if not nprocesses is None else '*'} number of processes minus 1",
        file=file,
    )
    print(
        f"imax {nnuisanceparams if not nnuisanceparams is None else '*'} number of nuisance paramaeters",
        file=file,
    )
    return


def write_observed(file: IO[str], channels: list[str], observed: list[float]):
    print("bin".ljust(len("observation")) + "   " + "   ".join(channels), file=file)

    observedLine: list[str] = []
    for chan, obs in zip(channels, observed):
        observedLine.append(str(obs).ljust(len(chan)))

    print("observation   " + "   ".join(observedLine), file=file)
    return


def write_block_separation(file: IO[str], length: int = 80):
    print("-" * length, file=file)


@dataclass
class Process:
    name: str
    channel: str
    isSignal: bool
    rate: float
    systematics: dict[str, tuple[str, float]] = field(
        default_factory=lambda: {"default": ("lnN", 1.01)}
    )
    columnWidth: int = 0

    def calculateWidth(self):
        self.columnWidth = len(
            str(
                max(
                    [
                        self.name,
                        self.channel,
                        self.rate,
                        *map(lambda x: x[1], self.systematics.values()),
                    ],
                    key=lambda x: len(str(x)),
                )
            )
        )


def extract_systematics(processes: list[Process]) -> list[tuple[str, str]]:
    allSystematics = set[tuple[str, str]]()
    for process in processes:
        for systematic in process.systematics.keys():
            allSystematics.add((systematic, process.systematics[systematic][0]))
    return list(allSystematics)


def extract_channels(processes: list[Process]) -> list[str]:
    channels = set([process.channel for process in processes])
    return list(channels)


def write_processes(file: IO[str], processes: list[Process]):

    systematicsAndTypes = extract_systematics(processes)
    systematics = list(map(lambda x: x[0], systematicsAndTypes))
    systType = list(map(lambda x: x[1], systematicsAndTypes))

    maxSystLength = len(max(systematics, key=lambda x: len(str(x))))
    maxTypeLength = len(max(systType, key=lambda x: len(str(x))))

    channelLine: str = "bin".ljust(maxSystLength + maxTypeLength + 1) + "   "
    processesChannel = [
        process.channel.ljust(process.columnWidth) for process in processes
    ]
    channelLine += "   ".join(processesChannel)

    processLine = "process".ljust(maxSystLength + maxTypeLength + 1) + "   "
    processesName = [process.name.ljust(process.columnWidth) for process in processes]
    processLine += "   ".join(processesName)

    rateLine: str = "rate".ljust(maxSystLength + maxTypeLength + 1) + "   "
    processesRate = [
        str(process.rate).ljust(process.columnWidth) for process in processes
    ]
    rateLine += "   ".join(processesRate)

    processIdxLine: str = "process".ljust(maxSystLength + maxTypeLength + 1) + "   "

    currSignal = 0
    currBkg = 1
    processesIdx: list[str] = []

    for process in processes:
        if process.isSignal:
            processesIdx.append(f"{currSignal}".ljust(process.columnWidth))
            currSignal -= 1
        else:
            processesIdx.append(f"{currBkg}".ljust(process.columnWidth))
            currBkg += 1

    processIdxLine += "   ".join(processesIdx)

    print(channelLine, file=file)
    print(processLine, file=file)
    print(processIdxLine, file=file)
    print(rateLine, file=file)

    write_block_separation(file)

    for systIdx, systematic in enumerate(systematics):
        systematicsLine: str = (
            systematic.ljust(maxSystLength) + " " + systType[systIdx] + "   "
        )
        processesSystematics = [
            (
                str(process.systematics[systematic][1]).ljust(process.columnWidth)
                if systematic in process.systematics
                else "-".ljust(process.columnWidth)
            )
            for process in processes
        ]
        systematicsLine += "   ".join(processesSystematics)

        print(systematicsLine, file=file)

    return


def write_data_card(
    file_path: str, processes: list[Process], observedDict: dict[str, float]
):

    channels = extract_channels(processes)
    nbin = len(channels)
    observed = [observedDict[channel] for channel in channels]

    with open(file_path, "w") as file:

        write_header(file, nbin)

        write_block_separation(file)
        write_block_separation(file)

        write_observed(file, channels, observed)

        write_block_separation(file)

        write_processes(file, processes)
    return


def data_cards_from_summaryPlot(config: PlotsKwarg, canvas: TCanvas, dir: str, version:int):
    for catIdx, category in enumerate(config.get("categories", [])):
        processList = []
        totalExpected = 0
        for modeIdx, mode in enumerate(config.get("modes", [])):
            curr = float(canvas._hists[modeIdx].GetBinContent(catIdx + 1))
            totalExpected += curr
            currProcess = Process(
                name=mode,
                channel=category,
                isSignal=bool(mode in config.get("signalPerCategory", {})[category]),
                rate=round(curr, decimals=3),
            )
            currProcess.calculateWidth()
            processList.append(currProcess)

        write_data_card(
            file_path=f"{dir}/data-cards/STXS{version if version == 0 else f'1p{version}'}_{category}.txt",
            processes=processList,
            observedDict={category: round(totalExpected, 3)},
        )
