{ inputs, ... }:
{
  imports = [
    inputs.treefmt-nix.flakeModule
  ];

  perSystem =
    {
      pkgs,
      system,
      ...
    }:
    let
      constants = import ../constants.nix { inherit system; };
      clangTools = pkgs."llvmPackages_${constants.clangVersion}".clang-tools.override {
        enableLibcxx = true;
      };
    in
    {
      treefmt = {
        programs = {
          nixfmt.enable = true;
          clang-format = {
            enable = true;
            package = clangTools;
          };
          prettier.enable = true;
          shfmt = {
            enable = true;
            indent_size = 4;
          };
          just.enable = true;
        };

        settings.excludes = [
          "adventofcode2020/**"
          "adventofcode2021"
          "adventofcode2022"
          ".gitignore"
          ".gitmodules"
          ".prettierignore"
          "LICENSE"
        ];

        settings.formatter.prettier.includes = [
          "**/.clang-tidy"
          ".clang-format"
          ".clang-tidy"
          ".clangd"
          ".prettierrc"
          "**/flake.lock"
          "flake.lock"
        ];

        settings.formatter.cmake-format = {
          command = "${pkgs.cmake-format}/bin/cmake-format";
          options = [ "-i" ];
          includes = [
            "CMakeLists.txt"
            "CMakeToolchain.*.txt"
            "*.cmake"
          ];
        };
      };
    };
}
