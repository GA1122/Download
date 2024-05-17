bool HTMLFormElement::HasLegalLinkAttribute(const QualifiedName& name) const {
  return name == actionAttr || HTMLElement::HasLegalLinkAttribute(name);
}
