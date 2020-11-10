with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "gamagora-realtime";

  hardeningDisable = [ "fortify" ];

  buildInputs = [glfw glm cimg x11];
}
