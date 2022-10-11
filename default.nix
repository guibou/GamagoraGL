with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "gamagora-realtime";

  hardeningDisable = [ "fortify" ];

  buildInputs = [cmake glfw glm cimg xlibsWrapper];
}
