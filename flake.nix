{
  description = "C++ Template";

  inputs = {
    nixpkgs.url = "nixpkgs";
    systems.url = "github:nix-systems/x86_64-linux";
    flake-utils = {
      url = "github:numtide/flake-utils";
      inputs.systems.follows = "systems";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    # For more information about the C/C++ infrastructure in nixpkgs: https://nixos.wiki/wiki/C
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        pname = "STXS-Categorize"; # package name
        version = "0.0.1";
        src = ./.;

        zzanalysis-headers = pkgs.stdenv.mkDerivation {
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
            mkdir -p $out/include/ZZAnalysis/AnalysisStep/src
            mkdir -p $out/share/ZZAnalysis/AnalysisStep/data/cconstants

            cp AnalysisStep/interface/cConstants.h \
            AnalysisStep/interface/Discriminants.h \
            $out/include/ZZAnalysis/AnalysisStep/interface

            cp AnalysisStep/src/cConstants.cc \
            AnalysisStep/src/Discriminants.cc \
            $out/include/ZZAnalysis/AnalysisStep/src

            cp AnalysisStep/data/cconstants/* \
            $out/share/ZZAnalysis/AnalysisStep/data/cconstants
          '';
        };
        buildInputs = with pkgs; [
          # add library dependencies here i.e.
          #zlib
          root
          zzanalysis-headers
          # Tip: you can use `nix-locate foo.h` to find the package that provides a header file, see https://github.com/nix-community/nix-index
        ];
        nativeBuildInputs = with pkgs; [
          # add build dependencies here
          ## For mesonbuild:
          #meson ninja
          ## For cmake:
          cmake
          ## For autotools:
          # autoconf-archive
          # autoreconfHook
          pkg-config
          # clangd language server.
          # Also start your IDE/editor from the shell provided by `nix develop` as the wrapped clangd from clang-tools needs environment variables set by the shell
          #clang-tools
        ];
      in
      {
        devShells.default = pkgs.mkShell {
          inherit buildInputs nativeBuildInputs;

          shellHook = ''
            export ROOT_RDF_SNAPSHOT_INFO=0
            export CPATH=${zzanalysis-headers}/include:$CPATH
            export ZZANALYSIS_DATA=${zzanalysis-headers}/share/ZZAnalysis/AnalysisStep
          '';

          # You can use NIX_CFLAGS_COMPILE to set the default CFLAGS for the shell
          #NIX_CFLAGS_COMPILE = "-g";
          # You can use NIX_LDFLAGS to set the default linker flags for the shell
          #NIX_LDFLAGS = "-L${lib.getLib zstd}/lib -lzstd";
        };

        # Pinned gcc: remain on gcc10 even after `nix flake update`
        #default = pkgs.mkShell.override { stdenv = pkgs.gcc10Stdenv; } {
        #  inherit buildInputs nativeBuildInputs;
        #};

        # Clang example:
        #default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
        #  inherit buildInputs nativeBuildInputs;
        #};

        packages = {
          categorize = pkgs.stdenv.mkDerivation {
            inherit
              buildInputs
              nativeBuildInputs
              pname
              version
              src
              ;
            cmakeFlags = [
              "-DZZANALYSIS_INCLUDE_DIR=${zzanalysis-headers}/include"
            ];
          };
          default = self.packages.${system}.categorize;
        };
      }
    );
}
