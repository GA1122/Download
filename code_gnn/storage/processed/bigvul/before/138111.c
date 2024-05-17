float AXNodeObject::stepValueForRange() const {
  if (!isNativeSlider())
    return 0.0;

  Decimal step =
      toHTMLInputElement(*getNode()).createStepRange(RejectAny).step();
  return step.toString().toFloat();
}
