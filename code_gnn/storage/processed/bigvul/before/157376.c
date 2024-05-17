bool HTMLMediaElement::IsURLAttribute(const Attribute& attribute) const {
  return attribute.GetName() == srcAttr ||
         HTMLElement::IsURLAttribute(attribute);
}
