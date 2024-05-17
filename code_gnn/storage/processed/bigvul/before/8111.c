void Gfx::opCloseEOFillStroke(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    return;
  }
  if (state->isPath() && !contentIsHidden()) {
    state->closePath();
    if (state->getFillColorSpace()->getMode() == csPattern) {
      doPatternFill(gTrue);
    } else {
      out->eoFill(state);
    }
    if (state->getStrokeColorSpace()->getMode() == csPattern) {
      doPatternStroke();
    } else {
      out->stroke(state);
    }
  }
  doEndPath();
}
