{
  perSystem =
    {
      self',
      ...
    }:
    {
      packages = {
        trainBDT = self'.packages.baseProgram.overrideAttrs (
          final: prev: {
            src = ./..;
            pname = prev.pname + "BDT";
            cmakeFlags = prev.cmakeFlags ++ [ "-DBUILD_CATEGORIZEBDT=ON" ];
          }
        );
      };
    };
}
