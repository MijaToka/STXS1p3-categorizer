{
  perSystem =
    {
      self',
      ...
    }:
    {
      packages.cutBased = self'.packages.baseProgram.overrideAttrs (
        final: prev: {
          src = ./..;
          pname = prev.pname + "CutBase";
          cmakeFlags = prev.cmakeFlags ++ [ "-DBUILD_CATEGORIZE=ON" ];
        }
      );
    };
}
