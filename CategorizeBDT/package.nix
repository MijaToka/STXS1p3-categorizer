{
  perSystem =
    {
      pkgs,
      ...
    }:
    {
      packages =
        let
          src = ./..;
          buildInputs = with pkgs; [ root ];
          nativeBuildInputs = with pkgs; [
            cmake
            pkg-config
          ];
          preConfigure = ''
            ln -s ${src} ../STXSCategorizer
          '';
          pname = "STXS-Categorize-BDT";
          version = "0.0.1";
          cmakeFlags = [ "-DBUILD_CATEGORIZEBDT=ON" ];
        in
        {
          trainBDT = pkgs.stdenv.mkDerivation {
            inherit
              src
              pname
              version
              buildInputs
              nativeBuildInputs
              preConfigure
              cmakeFlags
              ;
          };
        };
    };
}
