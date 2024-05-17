GfxState::~GfxState() {
  int i;

  if (fillColorSpace) {
    delete fillColorSpace;
  }
  if (strokeColorSpace) {
    delete strokeColorSpace;
  }
  if (fillPattern) {
    delete fillPattern;
  }
  if (strokePattern) {
    delete strokePattern;
  }
  for (i = 0; i < 4; ++i) {
    if (transfer[i]) {
      delete transfer[i];
    }
  }
  gfree(lineDash);
  if (path) {
    delete path;
  }
  if (saved) {
    delete saved;
  }
  if (font) {
    font->decRefCnt();
  }
}
