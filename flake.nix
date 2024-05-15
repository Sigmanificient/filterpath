{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachSystem ["x86_64-linux"]
    (system: let
      pkgs = nixpkgs.legacyPackages.${system};
    in rec {
      formatter = pkgs.alejandra;

      devShells.default = pkgs.mkShell {
        inputsFrom = pkgs.lib.attrsets.attrValues packages;
        packages = with pkgs; [
          python3Packages.compiledb
          gcovr
        ];
      };

      packages = rec {
        filterpath = default;
        default = pkgs.stdenvNoCC.mkDerivation {
          name = "filterpath";
          src = ./.;

          makeFlags = [
            "CC=${pkgs.stdenv.cc}/bin/cc"
            "PREFIX=${placeholder "out"}/bin"
          ];
        };
      };
    });
}
