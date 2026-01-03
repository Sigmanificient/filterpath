{
  stdenv,
  lib,
}:
stdenv.mkDerivation (finalAttrs: {
  name = "filterpath";
  version = "1.0.3";

  src = ./.;

  makeFlags = [
    "CC=cc"
    "PREFIX=${placeholder "out"}/bin"
  ];

  doCheck = true;
  checkPhase = ''
    runHook preCheck

    echo "[`pwd`]" | ./filterpath | grep "`pwd`"

    runHook postCheck
  '';

  meta = {
    homepage = "https://github.com/Sigmanificient/filterpath";
    description = "Retrieve a valid path from a messy piped line";
    license = lib.licenses.gpl3Only;
    maintainers = with lib.maintainers; [sigmanificient];
    mainProgram = "filterpath";
    platforms = lib.platforms.linux;
  };
})
