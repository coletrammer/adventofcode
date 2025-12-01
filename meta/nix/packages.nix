{ inputs, ... }:
{
  perSystem =
    { pkgs, system, ... }:
    let
      version = "0.1.0";
      mkPackage =
        stdenv: name: dius:
        stdenv.mkDerivation {
          name = "${name}-${version}";
          version = version;
          src = ../../.;

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
          ];

          buildInputs = [
            inputs.dius.packages.${system}.${dius}
          ];
        };

      mkApp = pkg: {
        type = "app";
        program = "${pkg}/bin/aoc";
      };

      aoc = mkPackage pkgs.stdenv "aoc-unwrapped" "dius-runtime";

      aoc-app = mkApp aoc;
    in
    {
      apps = {
        default = aoc-app;
        aoc = aoc-app;
      };
      packages = {
        default = aoc;
        aoc = aoc;
      };
    };
}
