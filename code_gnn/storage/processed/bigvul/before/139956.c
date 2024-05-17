LayoutObject* HTMLMediaElement::createLayoutObject(const ComputedStyle&) {
  return new LayoutMedia(this);
}
