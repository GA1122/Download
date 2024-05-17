DOMImplementation& Document::implementation() {
  if (!implementation_)
    implementation_ = DOMImplementation::Create(*this);
  return *implementation_;
}
