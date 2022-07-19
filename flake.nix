{
  inputs = {
    fake6502.url = "git+file:///home/matt/code/fake6502";
  };
  outputs = { self, nixpkgs, fake6502 }: {
    devShells.x86_64-linux.default = let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
    in pkgs.mkShell {
      buildInputs = with pkgs; [
        pkg-config
        cc65
        fake6502.packages.x86_64-linux.fake6502
      ];
      # hack: I can't figure out how to expose fake6502 properly
      shellHook = ''
        rm lib include
        ln -fs ${fake6502.packages.x86_64-linux.fake6502}/lib lib
        ln -fs ${fake6502.packages.x86_64-linux.fake6502}/include include
      '';
    };
  };
}
