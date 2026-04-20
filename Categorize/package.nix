{
  perSystem =
    {
      pkgs,
      self',
      ...
    }:
    {
      packages.categorize = pkgs.stdenv.mkDerivation {
        src = ./..;
        pname = "STXS-Categorize";
        version = "1.0.0";
        buildInputs = [
          pkgs.root
          self'.packages.zzanalysis-headers
        ];
        nativeBuildInputs = with pkgs; [
          cmake
          pkg-config
        ];

        cmakeFlags = [
          "-DZZANALYSIS_INCLUDE_DIR=${self'.packages.zzanalysis-headers}/include"
          "-DBUILD_CATEGORIZE=ON"
        ];

        preConfigure = ''
          ln -s ${self'.packages.categorize.src} ../STXSCategorizer
        '';
      };

      packages.zzanalysis-headers = pkgs.stdenv.mkDerivation {
        name = "zzanalysis-headers";
        src = pkgs.fetchFromGitHub {
          owner = "CJLST";
          repo = "ZZAnalysis";
          rev = "Run3";
          hash = "sha256-uSufJ/vXWmWV09PTRtvyMDAUSgnPafBWMusKPDlHlB4=";
        };
        dontBuild = true;
        installPhase = ''
          mkdir -p $out/include/ZZAnalysis/AnalysisStep/interface
          mkdir -p $out/share/ZZAnalysis/AnalysisStep/data/cconstants

          cp AnalysisStep/interface/{cConstants,Discriminants}.h \
          $out/include/ZZAnalysis/AnalysisStep/interface

          cp AnalysisStep/data/cconstants/* \
          $out/share/ZZAnalysis/AnalysisStep/data/cconstants
        '';
      };
    };
}
