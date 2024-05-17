void Gfx::opFillStroke(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    return;
  }
  if (state->isPath() && !contentIsHidden()) {
    if (state->getFillColorSpace()->getMode() == csPattern) {
      doPatternFill(gFalse);
    } else {
      out->fill(state);
    }
    if (state->getStrokeColorSpace()->getMode() == csPattern) {
      doPatternStroke();
    } else {
      out->stroke(state);
    }
  }
  doEndPath();
}
