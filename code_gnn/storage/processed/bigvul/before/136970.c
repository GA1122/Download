void HTMLInputElement::SetValueForUser(const String& value) {
  setValue(value, kDispatchChangeEvent);
}
