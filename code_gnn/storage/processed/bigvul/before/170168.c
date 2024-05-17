void PasswordAutofillAgent::PasswordValueGatekeeper::ShowValue(
    blink::WebInputElement* element) {
  if (!element->isNull() && !element->suggestedValue().isEmpty())
    element->setValue(element->suggestedValue(), true);
}
