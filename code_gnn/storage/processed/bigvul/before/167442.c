const AtomicString& SVGStyleElement::media() const {
  const AtomicString& n = FastGetAttribute(SVGNames::mediaAttr);
  return n.IsNull() ? MediaTypeNames::all : n;
}
