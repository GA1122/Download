void Gfx::opSetStrokeColorSpace(Object args[], int numArgs) {
  Object obj;
  GfxColorSpace *colorSpace;
  GfxColor color;

  state->setStrokePattern(NULL);
  res->lookupColorSpace(args[0].getName(), &obj);
  if (obj.isNull()) {
    colorSpace = GfxColorSpace::parse(&args[0], this);
  } else {
    colorSpace = GfxColorSpace::parse(&obj, this);
  }
  obj.free();
  if (colorSpace) {
    state->setStrokeColorSpace(colorSpace);
    out->updateStrokeColorSpace(state);
    colorSpace->getDefaultColor(&color);
    state->setStrokeColor(&color);
    out->updateStrokeColor(state);
  } else {
    error(getPos(), "Bad color space (stroke)");
  }
}
