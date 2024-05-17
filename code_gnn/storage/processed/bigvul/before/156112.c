bool HTMLLinkElement::HasLegalLinkAttribute(const QualifiedName& name) const {
  return name == hrefAttr || HTMLElement::HasLegalLinkAttribute(name);
}
