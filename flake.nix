{
  description = "C++ Template";

  inputs = {
    nixpkgs.url = "nixpkgs";
    systems.url = "github:nix-systems/x86_64-linux";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs =
    {
      flake-parts,
      ...
    }@inputs:
    flake-parts.lib.mkFlake { inherit inputs; }

      {
        imports = [
          ./Categorize/package.nix
          ./CategorizeBDT/package.nix
          ./package.nix
          ./shell.nix
        ];
        systems = [ "x86_64-linux" ];
      };
}
