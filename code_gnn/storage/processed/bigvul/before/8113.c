void Gfx::opClosePath(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    error(getPos(), "No current point in closepath");
    return;
  }
  state->closePath();
}
