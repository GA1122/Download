bool HTMLFormElement::ShouldAutocomplete() const {
  return !DeprecatedEqualIgnoringCase(FastGetAttribute(autocompleteAttr),
                                      "off");
}
