{
  perSystem =
    {
      pkgs,
      self',
      ...
    }:
    {
      packages = {
        baseProgram = pkgs.stdenv.mkDerivation {
          src = ./.;
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
          ];

          preConfigure = ''
            ln -s ${self'.packages.baseProgram.src} ../STXSCategorizer
          '';
        };

        default = self'.packages.baseProgram.overrideAttrs (
          final: prev: {
            cmakeFlags = prev.cmakeFlags ++ [
              "-DBUILD_CATEGORIZE=ON"
              "-DBUILD_CATEGORIZEBDT=ON"
            ];
          }
        );

        zzanalysis-headers = pkgs.stdenv.mkDerivation {
          name = "zzanalysis-headers";
          src = pkgs.fetchFromGitHub {
            owner = "CJLST";
            repo = "ZZAnalysis";
            rev = "Run3";
            hash = "sha256-7GYShs71BlhfqmeUOvwXWB0yJbM1UtySbG2i3fsclH4=";
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
    };
}
