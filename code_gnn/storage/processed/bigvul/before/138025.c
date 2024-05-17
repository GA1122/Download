String AXNodeObject::ariaAutoComplete() const {
  if (roleValue() != ComboBoxRole)
    return String();

  const AtomicString& ariaAutoComplete =
      getAOMPropertyOrARIAAttribute(AOMStringProperty::kAutocomplete).lower();

  if (ariaAutoComplete == "inline" || ariaAutoComplete == "list" ||
      ariaAutoComplete == "both")
    return ariaAutoComplete;

  return String();
}
