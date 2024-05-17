void HTMLInputElement::SetNonAttributeValueByUserEdit(
    const String& sanitized_value) {
  SetValueBeforeFirstUserEditIfNotSet();
  SetNonAttributeValue(sanitized_value);
  CheckIfValueWasReverted(sanitized_value);
}
