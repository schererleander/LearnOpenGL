{
  description = "LearnOpenGL development environment";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { nixpkgs, ... }:
    let
      forAllSystems = nixpkgs.lib.genAttrs [
        "x86_64-linux"
        "aarch64-linux"
        "aarch64-darwin"
      ];
    in
    {
      devShells = forAllSystems (system:
        let
          pkgs = import nixpkgs { inherit system; };
          inherit (pkgs) lib;

          isLinux = pkgs.stdenv.hostPlatform.isLinux;

          linuxRuntimeLibraries = with pkgs; [
            libGL
            wayland
            libxkbcommon
            libX11
            libxrandr
            libxinerama
            libxcursor
            libxi
          ];

          linuxPackages = with pkgs; [
            wayland-scanner
            wayland-protocols
          ];
        in
        {
          default = pkgs.mkShell (
            {
              packages =
                (with pkgs; [
                  cmake
                  pkg-config
                  libffi
                ])
                ++ lib.optionals isLinux (
                  linuxPackages ++ linuxRuntimeLibraries
                );
            }
            // lib.optionalAttrs isLinux {
              LD_LIBRARY_PATH =
                "${lib.makeLibraryPath linuxRuntimeLibraries}"
                + ":/run/opengl-driver/lib";
            }
          );
        });
    };
}
