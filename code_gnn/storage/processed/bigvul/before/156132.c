const QualifiedName& HTMLLinkElement::SubResourceAttributeName() const {
  if (DeprecatedEqualIgnoringCase(getAttribute(typeAttr), "text/css")) {
    return hrefAttr;
  }
  return HTMLElement::SubResourceAttributeName();
}
