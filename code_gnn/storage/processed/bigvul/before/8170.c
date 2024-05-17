void Gfx::opStroke(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    return;
  }
  if (state->isPath() && !contentIsHidden()) {
    if (state->getStrokeColorSpace()->getMode() == csPattern) {
      doPatternStroke();
    } else {
      out->stroke(state);
    }
  }
  doEndPath();
}
