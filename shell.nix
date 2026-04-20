{
  perSystem =
    {
      pkgs,
      self',
      ...
    }:
    {
      devShells.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          root
          (python313.withPackages (
            ps: with ps; [
              numpy
              root
            ]
          ))
        ];
        nativeBuildInputs = with pkgs; [
          cmake
          pkg-config
        ];
        shellHook = ''
          export ROOT_RDF_SNAPSHOT_INFO=0
          export CPATH=${self'.packages.zzanalysis-headers}/include:$CPATH
          export ZZANALYSIS_DATA=${self'.packages.zzanalysis-headers}/share/ZZAnalysis/AnalysisStep
        '';

      };
    };
}
