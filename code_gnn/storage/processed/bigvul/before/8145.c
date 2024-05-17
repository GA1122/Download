void Gfx::opSetFillColorSpace(Object args[], int numArgs) {
  Object obj;
  GfxColorSpace *colorSpace;
  GfxColor color;

  res->lookupColorSpace(args[0].getName(), &obj);
  if (obj.isNull()) {
    colorSpace = GfxColorSpace::parse(&args[0], this);
  } else {
    colorSpace = GfxColorSpace::parse(&obj, this);
  }
  obj.free();
  if (colorSpace) {
    if (textHaveCSPattern && drawText) {
      GBool needFill = out->deviceHasTextClip(state);
      out->endTextObject(state);
      if (needFill) {
        doPatternFill(gTrue);
      }
      out->restoreState(state);
    }
    state->setFillPattern(NULL);
    state->setFillColorSpace(colorSpace);
    out->updateFillColorSpace(state);
    colorSpace->getDefaultColor(&color);
    state->setFillColor(&color);
    out->updateFillColor(state);
    if (textHaveCSPattern) {
      out->beginTextObject(state);
      out->updateRender(state);
      out->updateTextMat(state);
      out->updateTextPos(state);
      textHaveCSPattern = colorSpace->getMode() == csPattern;
    } else if (drawText && out->supportTextCSPattern(state)) {
      out->beginTextObject(state);
      textHaveCSPattern = gTrue;
    }
  } else {
    error(getPos(), "Bad color space (fill)");
  }
}
