{
  description = "Async hello world in C++";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.${system}.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          cmake
          clang-tools
          llvmPackages.clang
        ];

        buildInputs = with pkgs; [
          llvmPackages.libcxx
          llvmPackages.libcxxStdenv

          folly.dev
          glog
          gflags
          libevent.dev
          double-conversion
          fmt.dev
        ];

        shellHook = ''
          export CC=${pkgs.clang}/bin/clang
          export CXX=${pkgs.clang}/bin/clang++
          echo "🔥You're in 🚀"
        '';
      };
    };
}
