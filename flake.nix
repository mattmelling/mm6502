{
  inputs = {
    fake6502.url = "git+file:///home/matt/code/6502/fake6502";
  };
  outputs = { self, nixpkgs, fake6502 }: {
    devShells.x86_64-linux.default = let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
    in pkgs.mkShell {
      nativeBuildInputs = with pkgs; [ pkg-config ];
      buildInputs = with pkgs; [
        cc65
        fake6502.packages.x86_64-linux.fake6502
      ];
    };
  };
}
