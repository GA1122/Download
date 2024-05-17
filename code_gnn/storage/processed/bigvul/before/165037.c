void HTMLCanvasElement::SetSize(const IntSize& new_size) {
  if (new_size == Size())
    return;
  ignore_reset_ = true;
  SetIntegralAttribute(kWidthAttr, new_size.Width());
  SetIntegralAttribute(kHeightAttr, new_size.Height());
  ignore_reset_ = false;
  Reset();
}
