void Gfx::doPatternStroke() {
  GfxPattern *pattern;

  if (!out->needNonText()) {
    return;
  }

  if (!(pattern = state->getStrokePattern())) {
    return;
  }
  switch (pattern->getType()) {
  case 1:
    doTilingPatternFill((GfxTilingPattern *)pattern, gTrue, gFalse);
    break;
  case 2:
    doShadingPatternFill((GfxShadingPattern *)pattern, gTrue, gFalse);
    break;
  default:
    error(getPos(), "Unimplemented pattern type (%d) in stroke",
	  pattern->getType());
    break;
  }
}
