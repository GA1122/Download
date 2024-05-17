int SVGElement::tabIndex() const {
  if (SupportsFocus())
    return Element::tabIndex();
  return -1;
}
