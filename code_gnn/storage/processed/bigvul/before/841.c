void SplashOutputDev::eoFill(GfxState *state) {
  SplashPath *path;

  if (state->getFillColorSpace()->isNonMarking()) {
    return;
  }
  path = convertPath(state, state->getPath());
  splash->fill(path, gTrue);
  delete path;
}
