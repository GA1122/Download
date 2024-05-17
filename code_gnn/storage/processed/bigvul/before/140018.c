bool HTMLMediaElement::isURLAttribute(const Attribute& attribute) const {
  return attribute.name() == srcAttr || HTMLElement::isURLAttribute(attribute);
}
