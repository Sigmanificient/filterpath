{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = {
    self,
    nixpkgs,
  }: let
    applySystems = nixpkgs.lib.genAttrs [
      "x86_64-linux"
      "aarch64-linux"
      "aarch64-darwin"
      "x86_64-darwin"
    ];
    eachSystem = f:
      applySystems (system:
        f {
          inherit system;
          pkgs = nixpkgs.legacyPackages.${system};
        });
  in {
    formatter = eachSystem ({pkgs, ...}: pkgs.alejandra);

    devShells.default = eachSystem ({
      pkgs,
      system,
    }: {
      default = pkgs.mkShell {
        inputsFrom = [self.packages.${system}.filterpath];

        env.CC = pkgs.stdenv.cc;

        packages = with pkgs; [
          python3Packages.compiledb
          gcovr
        ];
      };
    });

    packages = eachSystem ({
      pkgs,
      system,
    }: {
      filterpath = pkgs.callPackage ./. {};

      default = self.packages.${system}.filterpath;
    });
  };
}
