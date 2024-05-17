void PasswordAutofillAgent::PasswordValueGatekeeper::ShowValue(
    WebInputElement* element) {
  if (!element->IsNull() && !element->SuggestedValue().IsEmpty()) {
    element->SetAutofillValue(element->SuggestedValue());
    element->SetAutofillState(WebAutofillState::kAutofilled);
  }
}
