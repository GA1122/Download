void HTMLFormControlElement::copyNonAttributePropertiesFromElement(
    const Element& source) {
  HTMLElement::copyNonAttributePropertiesFromElement(source);
  setNeedsValidityCheck();
}
