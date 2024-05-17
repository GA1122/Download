const AtomicString& SVGStyleElement::type() const {
  DEFINE_STATIC_LOCAL(const AtomicString, default_value, ("text/css"));
  const AtomicString& n = getAttribute(SVGNames::typeAttr);
  return n.IsNull() ? default_value : n;
}
