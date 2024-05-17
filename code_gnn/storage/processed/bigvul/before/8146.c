void Gfx::opSetFillGray(Object args[], int numArgs) {
  GfxColor color;

  if (textHaveCSPattern && drawText) {
    GBool needFill = out->deviceHasTextClip(state);
    out->endTextObject(state);
    if (needFill) {
      doPatternFill(gTrue);
    }
    out->restoreState(state);
  }
  state->setFillPattern(NULL);
  state->setFillColorSpace(new GfxDeviceGrayColorSpace());
  out->updateFillColorSpace(state);
  color.c[0] = dblToCol(args[0].getNum());
  state->setFillColor(&color);
  out->updateFillColor(state);
  if (textHaveCSPattern) {
    out->beginTextObject(state);
    out->updateRender(state);
    out->updateTextMat(state);
    out->updateTextPos(state);
    textHaveCSPattern = gFalse;
  }
}
