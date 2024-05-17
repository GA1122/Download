void HTMLFormControlElement::CloneNonAttributePropertiesFrom(
    const Element& source,
    CloneChildrenFlag flag) {
  HTMLElement::CloneNonAttributePropertiesFrom(source, flag);
  SetNeedsValidityCheck();
}
