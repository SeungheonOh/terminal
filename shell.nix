with (import <nixpkgs> {});
mkShell {
  buildInputs = [
    meson
    ninja
    pkg-config
    glfw3
    fontconfig
    glew
    xorg.libX11
    ccls
    clang-tools
    lldb
  ];
}
