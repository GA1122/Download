void Gfx::opEndText(Object args[], int numArgs) {
  GBool needFill = out->deviceHasTextClip(state);
  out->endTextObject(state);
  drawText = gFalse;
  if (textHaveCSPattern) {
    if (needFill) {
      doPatternFill(gTrue);
    }
    out->restoreState(state);
  }
  textHaveCSPattern = gFalse;
}
