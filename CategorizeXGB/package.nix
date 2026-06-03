{
  perSystem =
    {
      self',
      ...
    }:
    {
      packages = {
        applyXGB = self'.packages.baseProgram.overrideAttrs (
          final: prev: {
            src = ./..;
            pname = prev.pname + "XGB";
            cmakeFlags = prev.cmakeFlags ++ [ "-DBUILD_CATEGORIZEXGB=ON" ];
          }
        );
      };
    };
}
