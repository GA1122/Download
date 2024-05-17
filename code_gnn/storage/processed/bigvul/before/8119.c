void Gfx::opEOFill(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    return;
  }
  if (state->isPath() && !contentIsHidden()) {
    if (state->getFillColorSpace()->getMode() == csPattern) {
      doPatternFill(gTrue);
    } else {
      out->eoFill(state);
    }
  }
  doEndPath();
}
