with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "gamagora-realtime";

  hardeningDisable = [ "fortify" ];

  NIX_CFLAGS_COMPILE="-Iglad/include -ICimg/include -Itinyply/include";
  buildInputs = [glfw glm cimg x11];
}
