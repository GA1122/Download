LayoutObject* HTMLMediaElement::CreateLayoutObject(const ComputedStyle&) {
  return new LayoutMedia(this);
}
