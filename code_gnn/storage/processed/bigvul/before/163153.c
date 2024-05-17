LayoutObject* HTMLIFrameElement::CreateLayoutObject(const ComputedStyle&) {
  return new LayoutIFrame(this);
}
