bool HTMLFormElement::IsURLAttribute(const Attribute& attribute) const {
  return attribute.GetName() == actionAttr ||
         HTMLElement::IsURLAttribute(attribute);
}
