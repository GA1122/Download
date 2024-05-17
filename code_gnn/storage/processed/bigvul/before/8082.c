void Gfx::doPatternFill(GBool eoFill) {
  GfxPattern *pattern;

  if (!out->needNonText()) {
    return;
  }

  if (!(pattern = state->getFillPattern())) {
    return;
  }
  switch (pattern->getType()) {
  case 1:
    doTilingPatternFill((GfxTilingPattern *)pattern, gFalse, eoFill);
    break;
  case 2:
    doShadingPatternFill((GfxShadingPattern *)pattern, gFalse, eoFill);
    break;
  default:
    error(getPos(), "Unimplemented pattern type (%d) in fill",
	  pattern->getType());
    break;
  }
}
